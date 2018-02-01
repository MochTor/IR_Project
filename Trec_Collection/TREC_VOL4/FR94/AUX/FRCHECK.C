#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern char *procargs(int, char **);
extern void frcheck(FILE *, char *);

main(int argc, char **argv)
{
char *finp; FILE *ip;

   finp = procargs(argc, argv);
   if ((ip = fopen(finp, "r")) == NULL)
      syserr(argv[0], "fopen", finp);
   frcheck(ip, finp);
   if (fclose(ip))
      syserr(argv[0], "fclose", "ip file stream"); free(finp);
}


void frcheck(FILE *ip, char *finp)
{
int c, linenum = 1;

   for ( c = fgetc(ip) ; !feof(ip) ; c = fgetc(ip) )
   {
      if (c == 0012)
         linenum++;
      else
      if (!isprint(c))
         fprintf(stderr, "non printing %04o on line %05d\n", c, linenum);
   }
}


char *procargs(int argc, char **argv)
{
   if (argc != 2) 
      fatalerr("Usage", argv[0], "gpofile.xxx");
   return strdup(argv[1]);
}
