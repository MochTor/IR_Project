/*    frscan.c
 *    look ahead routines, search for specific octal byte patterns.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "frio.h"

/* grab the next nmax digits from the file, if one is not a digit then	*/
/* stop short. convert this string from ascii chars into a single int	*/
int scan_tagint(FILE *ip, int nmax)
{
int c;
char *a, *b;

   if (!nmax)
      return -1;

   if ((a = b = (char *)malloc(nmax * sizeof(int))) == NULL)
      syserr("scan_tagint", "malloc", "space for int string");

   for ( c = bfgetc(ip) ; !bfeof(ip) && nmax ; c = bfgetc(ip) )
   {
      if (isdigit(c))
         *b++ = (char)c, nmax--;
      else
      if (c == 0256)
         handle_256seq(ip);
   }

   *b = '\0';
   if (c != bungetc(c, ip))
      fprintf(stderr, "scan_tagint: bungetc failed on %03o\n", c);
   c = atoi(a);
   free(a);
   return c;
}


/* given a stream ip, the routine reads n more chars, compares all of	*/
/* them one-for-one against s, then returns true if the search string	*/
/* was found. If it was not then the file is rewound with bungetc so 	*/
/* that nothing apparently happened					*/
int scan_immed(FILE *ip, char *octs, int searchlen)
{
char *s;
int i, found = 0, *want, *have;

   if ((have = (int *)malloc(searchlen * sizeof(int))) == NULL)
      syserr("scan_str", "malloc", "space for str stack");
   if ((want = (int *)malloc(searchlen * sizeof(int))) == NULL)
      syserr("scan_str", "malloc", "space for str stack");

   /* decimal numbers supplied as octal	codes in a string		*/
   for ( i = 0, s = octs ; i < searchlen ; i++ )
   { sscanf(s, "%o", &want[i]); while (isdigit(*s++)); }

   for ( i = 0 ; i < searchlen ; i++ )
   {
      have[i] = bfgetc(ip);
      if (bfeof(ip))
         fprintf(stderr, "scan_immed: bfeof after bfgetc\n");
   }

   for ( found = 1, i = 0 ; i < searchlen ; i++ )
      found = found && (have[i] == want[i]);

   if (!found)
   while (i-- && !bfeof(ip))
      if (have[i] != bungetc(have[i], ip))
         fprintf(stderr, "scan_immed: bungetc failed on %03o\n", have[i]);

   free(want); free(have);
   return found;
}



/* look from here on in the file "ip" for the word whose "searchlen"	*/
/* characters are specified as octal codes contained in a string.	*/
/* sscanf the string with successive %o to make an array of ints.	*/
/* look for a complete match but go no further than "searchlimit" chars	*/
/* ahead. always leave the file pointer rewound all the way back so 	*/
/* that nothing has changed						*/
int scan_soon(FILE *ip, char *octs, int searchlen, int searchlimit)
{
char *s;
int i, j, found, *want, *have;

   if ((have = (int *)malloc(searchlimit * sizeof(int))) == NULL)
      syserr("scan_str", "malloc", "space for str stack");
   if ((want = (int *)malloc(searchlen   * sizeof(int))) == NULL)
      syserr("scan_str", "malloc", "space for str stack");

   /* decimal numbers supplied as octal	codes in a string		*/
   for ( i = 0, s = octs ; i < searchlen ; i++ )
   { sscanf(s, "%o", &want[i]); while (isdigit(*s++)); }
   for ( j = 0 ; j < searchlimit ; j++ )
   {
      have[j] = bfgetc(ip);
      if (bfeof(ip))
         break;
   }

   for ( i = found = 0 ; i < j ; i++ )
   {
      found = (have[i] == want[found]) ? found+1 : 0;
      if (found == searchlen)
         break;		/* found the string we were looking for	*/
   }

   while (j--)
      if ((i = bungetc(have[j], ip)) != have[j])
          fprintf(stderr, "scan_soon: bungetc failed %d %03o\n", i, have[j]);

   free(want); free(have);
   return (found == searchlen);
}


/* look forward starting at the the next character in the stream ip	*/
/* for the sequence of characters from the file corresponding to the	*/
/* octal codes specified as a string octs				*/
/* the call scan_shrs(ip, "150 145 154 154 157", 24); looks for	the	*/
/* string "hello" within the next 24 characters from the file "ip"	*/
/* see man 1 ascii for octal codes. I have not used a char * string	*/
/* with [a-z] etc in it because I need to look for non-printing, non-	*/
/* keyboard characters too						*/

int scan_to(FILE *ip, char *octs, int searchlen, int searchlimit)
{
char *s;
int i, *want, c;

   if ((want = (int *)malloc(searchlen * sizeof(int))) == NULL)
      syserr("scan_str", "malloc", "space for str stack");
   /* decimal numbers supplied as octal	codes in a string		*/
   for ( i = 0, s = octs ; i < searchlen ; i++ )
   { sscanf(s, "%o", &want[i]); while (isdigit(*s++)); }

   for ( i = 0, c = bfgetc(ip) ; searchlimit-- ; c = bfgetc(ip) )
   {
      if (bfeof(ip))
         return 0;
      i = (c == want[i]) ? i+1 : 0;
      if (i == searchlen)
         break;			/* found the string we were looking for	*/
   }

   free(want);
   return (i == searchlen);
}


/* read n characters from the file and void them unto the ether		*/
void scan_trash(FILE *ip, int n)
{
   for ( ; n-- && !bfeof(ip) ; (void)bfgetc(ip));
}



/* count the number of items identical to c that are in the stream	*/
/* pointed to by ip, assuming that ip has just returned one of that	*/
/* item input is        "cccccx"					*/
/* file pointer is now    ^    then routine returns 3 and leaves the	*/
/* file pointer pointing     ^ i.e. to the last c item			*/

int count_chr(FILE *ip, char c)
{
int n; char a;
   for ( n = 1, a = bfgetc(ip) ; !bfeof(ip) && a == c ; n++, a = bfgetc(ip));
   (void)bungetc(a, ip);
   return n;
}

int scan_eof(FILE *ip, int searchlimit)
{
   while (searchlimit-- && !bfeof(ip))
      (void)bfgetc(ip);
   return bfeof(ip);
}


/* convert a string consisting only of printable ascii characters to	*/
/* a string containing octal codes. thus ctooc("hello") gives this:	*/
/* "150 145 154 154 157" in malloced space and null terminated		*/
char *ctooc(char *s)
{
int i, n;
char *out, tmp[8];

   n = strlen(s);
   if ((out = (char *)calloc(n*4+1, sizeof(char))) == NULL)
      syserr("ctooc", "calloc", "space for octal coded string");
   for ( i = 0 ; i < n ; i++ )
   {
      sprintf(tmp, "%03o ", *s++);
      strcat(out, tmp);
      if (strlen(out) > n*4)
         fatalerr("ctooc", "strcat", "strlen out exceeded");
   }
   return out;
}
