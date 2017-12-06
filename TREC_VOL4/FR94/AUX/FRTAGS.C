/*    frtags.c
 *    handle the control tags defined by the 007  sequence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "frparse.h"
#include "frio.h"
#include "frproto.h"
#include "frstop.h"

/*
  I01  Dochead(F47{03,00,02,17}): Volume and Number field of FR doc head
       and day of week.
       Tables: Start of a line.
  I02  PartPage(F4717): Dates at heads of the FR.
       Tables: maybe for indent, see jan6 p803 col1
  I03  PartPage(F4717): The Part I, II, IV line itself in a f4717 environment.
       Tables: to more indent than I02, see jan6 p824 col1-3
  I04  PartPage(F4717): The Department heading that Part.
       Docend: Title of person signing that FR entry, and their branch/agency.
  I05  PartPage(F4717): the administartion within the department,
                 the division within the administration,
                 the relevant section,i.e the 4 CFR Part 504 line
                 what is disscribed thereafter jan6 p873
  I06  Docend: The name of the guy signing the entry.
  I07  Docend: Occurs once in January. jan7. Same as I06. Typo???
  I08  Corrections(S4734). GPO employee name as a comment. Invisible.
       Always preceded by S4734. Seems to be a one-liner.
  I09  Partpage: Always on an empty line. Vertical spacing.
       Footnote: Vert spacing between footnotes. jan6 p689 col1
  I10  Dochead(S4703): Tag for AGENCY ACTION SUMMARY DATES FOR FURTH... SUPPLEM
       and the text associated with those bits. see p661 Jan6 "CH Hendren"
  I11  The basic text tag of the FR. Ubiquitous. First word of first line
       in a paragrapoh is indented slightly.
  I12  Tables: Indent, see jan10 p1370 col2 I12 indent a. but I21 indent b
       must be a typo. Maybe not just tables

  I13  Tables: indentation see p765. jan6 top table "Nonlabor"

  I14  First word is idented. Lines after the first wraparound are indented
       more. See p1316 jan10 col2 "_$10 ... consulate ..."

  I15  No indent on first word, but lines after first wraparound are
       indented. see p1316 jan10 col1 "I-\n129". Also formula indent.

  I16  No indent of first word, but lines after first wraparound have big
       indentation. see p1761 jan12 col3 "I... II... III"

  I20  Smaller text. with big first indent.
       see "Sincerely" p 1731 col1 85% jan12 ja3.
  I21  Small text otherwise just like I11.
  I26  Small text, first line no indentation, thereafter big indentation
       see "Re: PSD..." p 1731 col1 45% jan12 ja3.

  I41  Dochead: Docket Number


  I59  Horizontal ruling after the ACTION: entry in a Notices bit
       one text column wide see p792 col3
  I69 I50 Thick and thin horizontal rulings, one column wide.
  I69 I52 One thin ruling see top col2 and col1 p795 Jan 6
  I69 I18 One thin ruling see bot col2 p795 Jan 6
  I69 I41 One thin ruling see top col3 p796

  I94 Page header on first page. Fed Reg.
  I95 Capitalizes lowers, and sets in bold. see p3671 Jan 26.
      for titles of subsections.
*/
#ifdef PERPETUATE_NATALIE_SKIP_DOCUMENT
static int in_table;
#endif

void handle_007Z(FILE *ip, FILE *op, int *count, char *finp)
{
int c, d;

#ifdef PERPETUATE_NATALIE_SKIP_DOCUMENT
#define NOLIMIT -1
   fflush(op);
   in_table = typsetags[cTAG].status == 1;
   scan_to(ip, "123 165 142 146 157 162 155 141 164 072", 10, NOLIMIT);
#else
   for ( c = bfgetc(ip) ; !bfeof(ip) ; c = bfgetc(ip) )
   {
      if (c != 007)	/* just trash what is here, all the operator	*/
         continue; 	/* sign-on details, terminate on 007[FSZ]	*/
      bungetc((d = bfgetc(ip)), ip);
      if ( !bfeof(ip) && (d == 'Z' || d == 'S' || d == 'F') )
      {
         bungetc(c, ip);
         break;
      }
      else
         fprintf(stderr, "handle_007Z: counter example: "
            "007 +%c+ != [FSZ] found file %s", d, finp);
   }
#endif

#ifdef PERPETUATE_NATALIE_DOCUMENT_DELIMITATION
   /* semantags should all be closed already but make sure	*/
   close_tags(op, &typsetags[ITAG], 2, count);
   close_tags(op, semantags, NSEMTAGS, count);
   close_tags(op, typsetags, NTYPTAGS, count);
   close_tags(op, dlimitags, NDOCTAGS, count);

   open_atag(op, &dlimitags[DOC], count, 1);
   bump_docno();
   echo_docno(op, &dlimitags[DOCNO], count);
   echo_parent(op, &dlimitags[PARENT], count);
   open_atag(op, &dlimitags[TEXT], count, 1);
#endif
}


void handle_Ttag(FILE *ip, FILE *op, int *count, char *finp)
{
   open_itag(op, &typsetags[ITAG], count, typsetags[ITAG].status,
   typsetags[ITAG].grid, scan_tagint(ip, FACE_LEN));
}



void handle_Itag(FILE *ip, FILE *op, int *count, char *finp)
{
int tagint, field, face, grid, stop;

   field = -1;
   tagint = scan_tagint(ip, ITAG_LEN);
   grid = typsetags[ITAG].grid;
   face = typsetags[ITAG].typeface;

   if (typsetags[STAG].status == 4717)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      return;
   }

   if (tagint == 94 || tagint == 69 )
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
   }
   else
   if (tagint == 50)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);

      if (typsetags[ITAG].statusminus2 == 94 &&
          typsetags[ITAG].statusminus1 == 69)
      {
         /* the I94I69 (I50 or I52) combos indicate the US Government	*/
         /* department running this doc in the Federal Register		*/
         fprintf(op, "\n"); *count = 0;
         open_atag(op, &semantags[DEPT], count, 1);
         stop = frparse(ip, op, count, finp, stop_dept);
         close_atag(op, &semantags[DEPT], count);
      }
   }
   else
   if (tagint == 52)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);

      switch (typsetags[STAG].status)
      {
         case 4703: 	/* In 4703 this line is a notice heading	*/
	         	/* in all others it is the CFR Part #	*/
            if (typsetags[ITAG].statusminus1 == 69 ||
                typsetags[ITAG].statusminus2 == 69 ||
                typsetags[ITAG].statusminus1 == 41 )
            {
               open_atag(op, &semantags[DOCTITLE], count, 1);
               stop = frparse(ip, op, count, finp, stop_doctitle);
               close_atag(op, &semantags[DOCTITLE], count);
            }
            else
            if (typsetags[ITAG].statusminus1 == 90 ||
                typsetags[ITAG].statusminus2 == 90)
            {
               open_atag(op, &semantags[DEPT], count, 1);
               stop = frparse(ip, op, count, finp, stop_dept);
               close_atag(op, &semantags[DEPT], count);
            }
            else
               ; /* Docket number */

            break;


         case 4701:
            if (typsetags[ITAG].statusminus1 == 90)
            {
               open_atag(op, &semantags[DEPT], count, 1);
               stop = frparse(ip, op, count, finp, stop_dept);
               close_atag(op, &semantags[DEPT], count);
            }
            break;

         case 4734:
            if (typsetags[ITAG].statusminus1 == 41)
            {
               open_atag(op, &semantags[DOCTITLE], count, 1);
               stop = frparse(ip, op, count, finp, stop_doctitle);
               close_atag(op, &semantags[DOCTITLE], count);
            }
            else
            if (typsetags[ITAG].statusminus1 == 18 ||
                scan_soon(ip, "103 106 122", 3, 10))	/* "CFR" next 	*/
            {
               open_atag(op, &semantags[CFRNO], count, 1);
               stop = frparse(ip, op, count, finp, stop_cfr);
               close_atag(op, &semantags[CFRNO], count);
            }
            break;

         case 4700:
         case 4702:
            if (scan_soon(ip, "103 106 122", 3, 10))	/* "CFR" next 	*/
            {
               open_atag(op, &semantags[CFRNO], count, 1);
               stop = frparse(ip, op, count, finp, stop_cfr);
               close_atag(op, &semantags[CFRNO], count);
            }
            break;

         default :
            fprintf(stderr, "handle_Itag: STAG has weird value %d\n",
               typsetags[STAG].status);
      }
   }
   else
   if (tagint == 18)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      if (typsetags[ITAG].statusminus1 == 69 ||
          typsetags[ITAG].statusminus2 == 69 ||
          typsetags[ITAG].statusminus1 == 52 )
      {
         open_atag(op, &semantags[BUREAU], count, 1);
         stop = frparse(ip, op, count, finp, stop_bureau);
         close_atag(op, &semantags[BUREAU], count);
      }
   }
   else
   if (tagint == 56 || tagint == 55)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      if (typsetags[ITAG].statusminus1 == 69 ||
          typsetags[ITAG].statusminus2 == 69)
      {
         open_atag(op, &semantags[DOCTITLE], count, 1);
         stop = frparse(ip, op, count, finp, stop_doctitle);
         close_atag(op, &semantags[DOCTITLE], count);
      }
   }
   else
   if (tagint == 91 || tagint == 41)
   {
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      open_atag(op, &semantags[RINDOCKET], count, tagint);
      stop = frparse(ip, op, count, finp, stop_rindocket);
      close_atag(op, &semantags[RINDOCKET], count);
   }
   else
   /* look for 007 I 10 007 T 2		This is a special case	*/
   /* in which typesetting information indicates semantic info	*/
   /* about the Federal Register. Specifically its fields and	*/
   /* only its fields are so flagged. AGENCY SUMMARY ACTION	*/
   /* FOR FURTHER INFORMATION DATES EFFECTIVE DATES etc..	*/
   if (tagint == 10 && scan_immed(ip, "007 124 62", 3))
   {
      face = 2;
      if (scan_soon(ip, "101 107 105 116 103 131", 6, 16) ||
          scan_soon(ip, "141 147 145 156 143 171", 6, 16))	/* AGENCY */
          field = AGENCY;
      else
      if (scan_soon(ip, "101 103 124 111 117 116", 6, 16) ||
          scan_soon(ip, "141 143 164 151 157 156", 6, 16))	/* ACTION */
          field = ACTION;
      else
      if (scan_soon(ip, "101 104 104 122 105 123 123", 7, 16) ||
          scan_soon(ip, "141 144 144 162 145 163 163", 7, 16))	/* ADDRESS */
          field = ADDRESS;
      else
      if (scan_soon(ip, "123 125 115 115 101 122 131", 7, 16) ||
          scan_soon(ip, "163 165 155 155 141 162 171", 7, 16))	/* SUMMARY */
          field = SUMMARY;
      else						/* FOR FURTH	*/
      if (scan_soon(ip, "106 117 122 040 106 125 122 124 110", 9, 16) ||
          scan_soon(ip, "146 157 162 040 146 165 162 164 150", 9, 16))
          field = FURTHER;
      else
      if (scan_soon(ip, "123 125 120 120 114 105 115", 7, 16) ||
          scan_soon(ip, "163 165 160 160 154 145 155", 7, 16))	/* SUPPLEM */
          field = SUPPLEM;
      else
      if (scan_soon(ip, "124 111 115 105", 4, 16) ||
          scan_soon(ip, "164 151 155 145", 4, 16) ||		/* TIME */
          scan_soon(ip, "105 106 106 105 103 124 111", 7, 16) ||
          scan_soon(ip, "145 146 146 145 143 164 151", 7, 16) ||/* EFFECTI */
          scan_soon(ip, "104 101 124 105", 4, 16) ||
          scan_soon(ip, "144 141 164 145", 4, 16))		/* DATE */
          field = DATE;

      if (field != -1)
      {
         close_atag(op, &typsetags[QTAG], count);
         close_atag(op, &typsetags[ITAG], count);
         close_tags(op, semantags, NSEMTAGS, count);
         open_atag(op, &semantags[field], count, 1);
      }
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
   }
   else
   if (tagint == 4 || tagint == 6 || tagint == 40 || tagint == 84)
   {
      if (tagint == 6 && !typsetags[cTAG].status) /* no word to key on as */
         field = SIGNATORY;			/* it is someone's name	*/
      else
      if (tagint == 4 && !typsetags[cTAG].status) /* I04 outside table	*/
         field = SIGNATJOB;
      else
      if (tagint == 40 && !typsetags[cTAG].status) /* [FR Doc. 94-765 .. ] */
         field = FRDOCFILING;			/* indicates the end	*/
      else
      if (tagint == 84 && scan_immed(ip, "007 124 62", 3) && /* 007 T 2	*/
         (scan_soon(ip, "123 125 120 120 114 105 115", 7, 16) ||
          scan_soon(ip, "163 165 160 160 154 145 155", 7, 16) ))
         face = 2, field = SUPPLEM;		/* SUPPLEM		*/
 
      if (field != -1)
      {
         close_atag(op, &typsetags[QTAG], count);
         close_atag(op, &typsetags[ITAG], count);
         close_tags(op, semantags, NSEMTAGS, count);
         open_atag(op, &semantags[field], count, 1);
      }
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
   }
   else
   if (tagint == 68)					/* BILLING CODE	*/
   {
      /* Before we close all mutually exclusive semantic field level	*/
      /* tags as is the normal practice, check if the last such tag	*/
      /* encountered was the FR Doc No tag. Because the combination of	*/
      /* the FR Doc. and BILLING lines is sufficient to finish a doc	*/
      /* then we should parsre the BILLING line	and close up the 	*/
      /* document delimiting tags.					*/

      int the_end = semantags[FRDOCFILING].status == 1;

      close_atag(op, &typsetags[QTAG], count);
      close_atag(op, &typsetags[ITAG], count);
      close_tags(op, semantags, NSEMTAGS, count);
      open_atag(op, &semantags[BILLING], count, 1);
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      stop = frparse(ip, op, count, finp, stop_billing);
      close_atag(op, &typsetags[ITAG], count);
      close_atag(op, &semantags[BILLING], count);


#ifdef PERPETUATE_NATALIE_DOCUMENT_DELIMITATION
      if (the_end)
      {
         if (!scan_soon(ip, "007 132", 2, 44) && !scan_eof(ip, 44))
            fprintf(stderr, "Doc ends now, but 007 Z is not near\n");
      }

      if (the_end)
         bump_parent();
#else
      if (the_end)
      {
         close_tags(op, typsetags, NTYPTAGS, count);
         close_atag(op, &dlimitags[TEXT], count);
         close_atag(op, &dlimitags[DOC], count);

         open_atag(op, &dlimitags[DOC], count, 1);
         bump_docno();
         echo_docno(op, &dlimitags[DOCNO], count);
         bump_parent();
         echo_parent(op, &dlimitags[PARENT], count);
         open_atag(op, &dlimitags[TEXT], count, 1);
      }
#endif
   }
   else
   if (tagint == 28 && scan_immed(ip, "007 116", 2))	/* 007 N	*/
   {
      close_atag(op, &typsetags[ITAG], count);
      open_atag(op, &typsetags[NTAG], count, 1);

      if (scan_soon(ip, "377 061 101", 3, 16))	/* a valid terminator	*/
      {						/* for the foot label	*/
         fprintf(op, "\n<FOOTNAME>");
         open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
         stop = frparse(ip, op, count, finp, stop_footlabel);
         close_atag(op, &typsetags[ITAG], count);
         fprintf(op, "\n</FOOTNAME>");
      }

      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
      stop = frparse(ip, op, count, finp, stop_footnote);
      if (stop != STOP_ITERM)
         close_atag(op, &typsetags[ITAG], count);
      close_atag(op, &typsetags[NTAG], count);
   }
   else
      open_itag(op, &typsetags[ITAG], count, tagint, grid, face);
}

void close_tags(FILE *op, TAGS *tags, int ntags, int *count)
{
   while (ntags--)		/* reverse order is important	*/
      close_atag(op, &tags[ntags], count);
}

void close_atag(FILE *op, TAGS *tag, int *count)
{
char *s;
   /* if the tag is already closed then do nothing, else close it	*/
   if (!tag->status)
      return;

   tag->statusminus2 = tag->statusminus1;
   tag->statusminus1 = tag->status;
   *count += fprintf(op, tag->end), tag->status = 0;

   for ( s = tag->end ; *s ; s++ );	/* skip to last	char of the	*/
   if (*--s == '\n')			/* closing string. if it is \n 	*/
      *count = 0;			/* then set count to zero	*/
}

void open_atag(FILE *op, TAGS *t, int *count, int status)
{
int n;

   close_atag(op, t, count);
   n = fprintf(op, t->begin, t->status = status);
   *count = t->begin[0] == '\n' ?  0 : *count + n;
}


void open_itag(FILE *op, TAGS *t, int *count, int loc, int grid, int face)
{
int n;
   if ( loc == t->status && grid == t->grid && face == t->typeface )
      return;
   if ( loc == 0 && t->status == 0 )
      loc = (t->statusminus1 == 0) ?
           ((t->statusminus2 == 0) ? 11 : t->statusminus2) : t->statusminus1;

   close_atag(op, t, count);
   n = fprintf(op, t->begin, t->status = loc,
                   t->grid = grid, t->typeface = face);
   *count = t->begin[0] == '\n' ?  0 : *count + n;
}




/* inline special symbols. it is followed by 1 2 5 6 7 8 or 256... and	*/
/* is usually closed by a 007 K code					*/
void handle_Gtag(FILE *ip, FILE *op, int *count, char *finp)
{
int stop, face, grid = scan_tagint(ip, GRID_LEN);

   if (grid > 8 || grid < 1)
      fprintf(stderr, "unknown grid ![0-8] in file %s : %d\n", finp, grid);
        
   face = scan_immed(ip, "007 124", 2) ?
          scan_tagint(ip, FACE_LEN) : typsetags[ITAG].typeface;

   open_itag(op, &typsetags[ITAG], count, typsetags[ITAG].status, grid, face);
   stop = frparse(ip, op, count, finp, stop_Gspecial);
   if (stop == STOP_NEWLINE)		/* a ^GK not found explicity so	*/
      handle_Ktag(ip, op, count, finp);	/* synthesize the effect of one	*/
}


#define DEFAULT_GRID 1
#define DEFAULT_FACE 1

void handle_Ktag(FILE *ip, FILE *op, int *count, char *finp)
{
   open_itag(op, &typsetags[ITAG], count, typsetags[ITAG].status,
             DEFAULT_GRID, DEFAULT_FACE);
}



void handle_Stag(FILE *ip, FILE *op, int *count, char *finp)
{
   /* S4700, 4702, 4703 are the default STAG's for the r*[023].xxx	*/
   /* files. they indicate resumption of output following a 007Z!	*/
   /* gpo operator logon.						*/
   /* S4701 is a typo I think						*/
   /* S4706 indicates wide text, across all three columns.		*/
   /* S4725 indicates a file to be included externally			*/
   /* S4734 starts gpo operator comment in the corrections environment	*/
   /* S4761 special tag in table in r21ja3				*/

   close_tags(op, typsetags, NTYPTAGS, count);
   open_atag(op, &typsetags[STAG], count, scan_tagint(ip, STAG_LEN));
#ifdef PERPETUATE_NATALIE_SKIP_DOCUMENT
   if (in_table)
      open_atag(op, &typsetags[cTAG], count, 1), in_table = 0;
#endif
}


void handle_Ftag(FILE *ip, FILE *op, int *count, char *finp)
{
   /* F4700, 4702, 4703 establish the default page head echoed at the	*/
   /* top of each page for each of the r*[023].xxx files respectively	*/
   /* F4701 establishes a new head within any of those files for	*/
   /* subsequent Parts of the book.					*/
   /* F4700 Rules + Regs. F4702 Proposed Rules. F4703 Notices.		*/
   /* F4705 is Presidential Documents.					*/

   /* F4717 = Part page with the portrait "federal register" running	*/
   /* up the center of the page, and the Date vspace Part vspace Dept	*/
   /* etc. Occurs 85 times in January. Part II thru IX. No Part I	*/
   /* Contents of this environment are delimited by the a 007Z or a new	*/
   /* STAG or FTAG or by two newline returns */

   /* F4718 gives the header for Sunshine Act meetings' pages. 		*/
   /* F4734 gives the header for Corrections's pages			*/
   /* both these codes are terminated by two consecutive newlines	*/
   /* F4761 occurs once, appears to act as a F4701, a typo perhaps.	*/
   /* but may be a special environment for a weird table, see book!	*/

   close_tags(op, typsetags, NTYPTAGS, count);
   open_atag(op, &typsetags[FTAG], count, scan_tagint(ip, FTAG_LEN));
   open_atag(op, &typsetags[STAG], count, typsetags[FTAG].status); /*!!!*/

#ifndef PERPETUATE_NATALIE_DOCUMENT_DELIMITATION
   open_atag(op, &dlimitags[DOC], count, 1);
   echo_docno(op, &dlimitags[DOCNO], count);
   echo_parent(op, &dlimitags[PARENT], count);
   open_atag(op, &dlimitags[TEXT], count, 1);
#endif
}


/* this is just doing vertical spacing. the integer argument is	*/
/* just vertical space in points. this from Dave Turney at GPO	*/
/* 202 512 0676 April 8. 1996.					*/

void handle_Qtag(FILE *ip, FILE *op, int *count, char *finp)
{
   close_atag(op, &typsetags[ITAG], count);
   open_atag(op, &typsetags[QTAG], count, scan_tagint(ip, QTAG_LEN));
   close_atag(op, &typsetags[QTAG], count);
}

void handle_256seq(FILE *ip)
{
   scan_trash(ip, 5);
}

void handle_imports(FILE *ip, FILE *op, int *count, char *finp)
{
int stop;
   open_atag(op, &typsetags[gTAG], count, 1);
   *count = 0; fprintf(op, "\n<!-- PJG importformat ");
   stop = frparse(ip, op, count, finp, stop_importformat);
   if (stop != STOP_NEWLINE)
      fprintf(stderr, "importformat did not stop on newline %d\n", stop); 
   *count = 0; fprintf(op, " -->\n");
   stop = frparse(ip, op, count, finp, stop_graphic);
   close_tags(op, &typsetags[gTAG], 3, count);
}

/* this handles the table environment began by 007c and ended by 007e.	*/
void handle_starttable(FILE *ip, FILE *op, int *count, char *finp)
{
int stop;
   if (typsetags[cTAG].status)
      fprintf(stderr, "handle_starttable: cTAG already set: "
         "table in table??? file %s", finp);
   close_atag(op, &typsetags[QTAG], count);
   close_atag(op, &typsetags[ITAG], count);
   open_atag(op, &typsetags[cTAG], count, 1);
   /* everything in here between now and I95 is the "function line"	*/
   /* giving an exact specification of the table			*/

   *count = 0; fprintf(op, "\n<!-- PJG tableformat ");
   stop = frparse(ip, op, count, finp, stop_tableformat);
   *count = 0; fprintf(op, " -->\n");
}

void handle_endtable(FILE *ip, FILE *op, int *count, char *finp)
{
   if (!typsetags[cTAG].status)
      fprintf(stderr,
         "handle_endtable ERROR cTAG not set. Are we not in a table???");
   /* r25ap2.xxx is screwed in this respect */
   close_tags(op, &typsetags[cTAG], 4, count);
}

void handle_last(FILE *ip, FILE *op, int *count, char *finp)
{
int c = bfgetc(ip);
   if (!bfeof(ip))
   {
      for (fprintf(stderr, "odd eof %s:\n", finp) ; !bfeof(ip) ; c = bfgetc(ip))
         fprintf(stderr, "+%d+ ", c);
      fprintf(stderr, "\n");
   }
   close_tags(op, typsetags, NTYPTAGS, count);
   close_tags(op, semantags, NSEMTAGS, count);
   close_tags(op, dlimitags, NDOCTAGS, count);
}



static int docno = 0;
static int parentdocno = 1;

extern void bump_docno(void) { ++docno; }
extern void bump_parent(void) { ++parentdocno; }

extern void echo_docno(FILE *op, TAGS *docnotag, int *count)
{
   *count += fprintf(op, "%s %s-%05d %s",
             docnotag->begin, docstring, docno, docnotag->end);
}

extern void echo_parent(FILE *op, TAGS *parentag, int *count)
{
   *count += fprintf(op, "%s %s-%05d %s",
             parentag->begin, docstring, parentdocno, parentag->end);
}
