/*    parse.c
 *    This program will reformat the 1994 Federal Register
 *    Data and put it in SGML format (Does not add Docno field)
 *    Natalie Willman  
 *    Fixes problems associated with Natalie's parser. Handles
 *    control bytes more correctly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "frparse.h"
#include "frio.h"
#include "frproto.h"
#include "frstop.h"

main(int argc, char **argv)
{
FILE	*ip, *op;	/* handles for unparsed infile and sgml outfile	*/
char    *finp, *fout;	/* names of input GPO .xxx, output file		*/
int	stop, count = 0;

   finp = procargs(argc, argv);
   fout = setdocstring(finp);
   bio(finp);
   if ((ip = fopen(finp, "r")) == NULL) syserr(argv[0], "fopen", finp);
   if ((op = fopen(fout, "w")) == NULL) syserr(argv[0], "fopen", fout);
   stop = frparse(ip, op, &count, finp, stop_bfeof);
   if (stop != STOP_BFEOF && stop != STOP_GPOEOF)
      fprintf(stderr, "frparse top: unexpected stop code %d\n", stop);
   if (fclose(ip)) syserr(argv[0], "fclose", "ip file stream"); free(finp);
   if (fclose(op)) syserr(argv[0], "fclose", "op file stream"); free(fout);
}


/* read from the input and process it. stop when the supplied stopping	*/
/* function returns a nonzero value 					*/
int frparse(FILE *ip, FILE *op, int *count, char *finp, int stopparse())
{
int prev, this, next, more;
int stop, num006 = 0;

  for (this = bfgetc(ip) ; !(stop = stopparse(ip, this)) ; this = bfgetc(ip))
  {
     switch (this)
     {
        case 0000 :
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0001 :
        case 0002 :	/* does not occur in jan's r*.xxx files		*/
           if (typsetags[ITAG].grid == 5 || typsetags[ITAG].grid == 6)
              fprintf(op, "&bull;");
           else
              fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0003 :	/* horizontal ruling in a table environment	*/
			/* see the rTAG					*/
           if (!typsetags[cTAG].status)
              fprintf(op, "\n<!-- PJG %04o extable -->\n", this), *count = 0;
           break;

        case 0004 :	/* used to indicate fractions 1/2 or 1/4 with	*/
                        /* a weird syntax p777 r06ja3			*/
                        /* prevents cursor advance, same as \ p776	*/
           fprintf(stderr, "Skipping %04o fraction file %s\n", this, finp);
           break;

        case 0005 :	/* does not occur in Jan's r*.xxx files		*/
           fprintf(stderr, "Unknown byte %d in file %s\n", this, finp);
           break;

        case 0006 :	/* may be followed by 006, 007, 377, 040 or 061	*/
           num006 = 1 + (scan_immed(ip, "006", 1) == 1); /* count more	*/
           break;

        case 0011 :	/* a horizontal tab character 			*/
           fprintf(op, "\n<!-- PJG %04o horztab -->\n", this), *count = 0;
           break;

        case 0012 :	/* newline character				*/
#ifdef TTTT
           /* the next four lines are stylistic only. if a new I or T	*/
           /* code is imminent then close the current one up -before-	*/
           /* the newline. it would happen afterwards anyway but it is	*/
           /* prettier this way						*/
           if (scan_soon(ip, "007 111", 2, 4) ||	/* either 007IT	*/
               scan_soon(ip, "007 124", 2, 4)  )	/* either 007IT	*/
              close_atag(op, &typsetags[ITAG], count);	/* is very soon	*/
#endif


           fprintf(op, "\n<!-- PJG %04o frnewline -->\n", this), *count = 0;
           break; 

        case 0013 :	/* special cents character with line thru	*/
			/* p1725 col3 bottom, r12ja3			*/
                        /* contained in ^G ^K sequence 			*/
           *count += fprintf(op, "&cent;");
           break;

        case 0015 : 	/* carriage return				*/
           break;

        case 0016 :	/* does not occur in the jan r*.xxx files	*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0014 :	/* a latex "\P" paragraph symbol		*/
           *count += fprintf(op, "&para;");
           break;

        case 0017 :	/* degrees symbol angle, latitude		*/
			/* see p689 r06ja0.od-cv line 14831		*/
           (void)fprintf(op, "\n<!-- PJG %04o degreeslatlong -->\n", next);
           *count = 0;
           *count += fprintf(op, "&cir;");
           break;

        case 0020 :	/* minutes symbol angle, latitude		*/
			/* see p689 r06ja0.od-cv line 14831		*/
			/* repeated twice means seconds of arc		*/
           (void)fprintf(op, "\n<!-- PJG %04o minuteslatlong -->\n", next);
           *count = 0;
           *count += fprintf(op, "&rsquo;");
           break;

        case 0021 :	/* followed by 030 or 031 it means a bullet	*/
                        /* symbol in an unenumerated list		*/
           if (scan_immed(ip, "031", 1) || scan_immed(ip, "030", 1))
              *count += fprintf(op, "&bull;");
           break;

        case 0022 :	/* greek letter mu symbol \mu p1868 col1 bot	*/
           *count += fprintf(op, "&mu;");
           break;

        case 0023 : 	/* math fraction				*/
        case 0024 :	/* does not occur				*/
        case 0025 :	/* does not occur				*/
        case 0026 :	/* occurs only once. in january	r*.xxx		*/
			/* files on Jan10. role is unknown. ignore	*/
        case 0027 :	/* does not occur in the january r*.xxx		*/
			/* files so ignore if it does come along	*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0030 :
        case 0031 :	/* occurs in FR page head to indicate spacing	*/
           		/* before and after the / delimiter.		*/
           *count += fprintf(op, "&blank;");
           break;

        case 0032 :	/* this is always the last byte in the file	*/
           handle_last(ip, op, count, finp);
           break;

        case 0033 :	/* occurs only in r03ja0.xxx, meaning unknown	*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0034 :	/* forward spacing see p876 jan6 col2 para4	*/
			/* used to promote, but not force a line feed	*/
			/* see p4044 Jan28 col2 para3 r28ja3.od-cv 	*/
			/* line 10957. rare so just ignore for now.	*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0035 :	/* in a table environ. this means >		*/
			/* see Jan6 p852 col1+2				*/
           fprintf(op, ">");
           break;

        case 0036 :	/* does not occur in january r*.xxx		*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case 0037 :	/* does not occur in january r*.xxx		*/
           fprintf(op, "\n<!-- PJG %04o unknown -->\n", this), *count = 0;
           break;

        case '\\':	/* footnote delimiter sometimes			*/
           /* if we are in a footnote definintion environment then this	*/
           /* character is skipped, else we may be about to see a 	*/
           /* footnote citation which we need to record 		*/
           if (typsetags[NTAG].status != 1)
           {
              if (scan_soon(ip, "007 116", 2, 20) &&	/* 007 N	*/
                  scan_soon(ip, "134", 1, 10))		/* \\		*/
              handle_footcite(ip, op, count, finp);
           }
           break;

        case '"' :	/* if we are in a table environment then this	*/
			/* means the >= sign else it is just a quote	*/ 
			/* see Jan6 p852 col1+2				*/
           if (typsetags[cTAG].status) *count += fprintf(op, "&ge;");
                                  else *count += fprintf(op, "\"");
           break;

        case '>' :	/* if we are in a table envirnoment then this	*/
			/* means the < sign [sic] perverse		*/
                        /* else it means a > sign see Jan6 p852 col1+2	*/
           if (typsetags[cTAG].status) *count += fprintf(op, "&lt;");
                                  else *count += fprintf(op, "&gt;");
           break;

        case '<':
           *count += fprintf(op, "&lt;");
           break;

        case 0177 :	/* always followed by a 007K which closes a	*/
			/* 007G, special character for the resigtered	*/
			/* trademark symbol circled			*/
           *count += fprintf(op, "&reg;");
           break;

        case 0256 :	/* unknown role. always followed by		*/
			/* four letters M D ? ? then closed with 257	*/
			/* just skip them and ignore this for now.	*/
           handle_256seq(ip);
           break;
   
        case 0257 :	/* always preceeded by 256. function		*/
			/* unknown, so ignore it.			*/
           (void)fprintf(stderr, "257 found alone, apart from 256 MDBN\n");	
           break;

        case 0320 :	
           (void)fprintf(op, "\n<!-- PJG %04o forcenewline -->\n", this);
           *count = 0;
           break;

        case 0377 :	/* also and see 006 above. In order of freq	*/
			/* 377 0 9 mean long hyphen in phone nos	*/
           if (scan_immed(ip, "060 071", 2))
              *count += fprintf(op, "&hyph;");
           else
           /* 377 1 A means two possible things 			*/
           /* One: if we have just encountered octal byte 006 one or	*/
           /* times then this corresponds to the section \S symbol	*/
           /* being repeated that many times.
           /* Two: this corresponds to either the beginning or ending	*/
           /* of a footnote label. A footnote citation is labelled	*/
           /* beginning with a \ or with a 3771A and is terminated	*/
           /* by a 007 N. Conversely a footnote defintion is started	*/
           /* with a 007 N and terminated by a 3771A which is odd	*/
           if (scan_immed(ip, "061 101", 2))
           {
              if (num006)
                 while (num006-- > 0) *count += fprintf(op, "&sect;");
              else
              if (scan_soon(ip, "007 116", 2, 16) && /* 007 N and not	*/
                 !typsetags[NTAG].status)	/* in footnote already	*/
                 handle_footcite(ip, op, count, finp);
           }
           else
           /* 377 1 B is a # sign see p1653 jan12 col1			*/
           if (scan_immed(ip, "061 102", 2))
              *count += fprintf(op, "#");
           else
           /* 377 0 A is a multiply sign table p761 jan06 col1		*/
           if (scan_immed(ip, "060 101", 2))
              *count += fprintf(op, "&times;");
           else
           /* 377 A E the previous char had an accent			*/
           if (scan_immed(ip, "101 105", 2))
           {
              next = bfgetc(ip);
              if (next == 0256)
                 handle_256seq(ip), next = bfgetc(ip);
              else
              if (next == '1' && isprint(prev)) /* 377AE1 => acute  	*/
                 *count += fprintf(op, "&%cacute;", prev);
              else
              if (next == '2' && isprint(prev)) /* 377AE2 => grave  	*/
                 *count += fprintf(op, "&%cgrave;", prev);
              else
              if (next == '3' && isprint(prev)) /* 377AE3 => circumflex	*/
                 *count += fprintf(op, "&%ccirc;", prev);
              else
              if (next == '4' && isprint(prev)) /* 377AE4 => umlaut	*/
                 *count += fprintf(op, "&%cuml;", prev);
              else
              if (next == '5')
                 fprintf(stderr, "377 AE 5 found. egrep did not. %s\n", finp);
              else
              if (next == '6' && isprint(prev)) /* 377AE6 => tilda  	*/
                 *count += fprintf(op, "&%ctilde;", prev);
              else
              if (next == '7')
                 fprintf(stderr, "377 AE 7 found. egrep did not. %s\n", finp);
              else
              if (next == '8')
                 fprintf(stderr, "377 AE 8 found. egrep did not. %s\n", finp);
              else
              if (next == '9' && isprint(prev)) /* 377AE9 => cedilla	*/
                 *count += fprintf(op, "&%ccedil;", prev);
              else
                 fprintf(stderr, "%s 377AE bad accent %04o %04o %04o\n",
                                  finp, prev, this, next);
           }
           else
           /* 377 7 E							*/
           if (scan_immed(ip, "067 105", 2))
              ;
           else
           /* 377 0 8							*/
           if (scan_immed(ip, "060 070", 2))
              ;
           else
           /* 377 1 1							*/
           if (scan_immed(ip, "061 061", 2))
              ;
           break;

        case 0007 :	/* control G is the a locator code		*/
           while ((next = bfgetc(ip)) == 0256)
              handle_256seq(ip);

           switch (next)
           {
              case 'F':	/* delimits Parts of FR.			*/
                 handle_Ftag(ip, op, count, finp);
                 break;
              case 'S':	/* subformats					*/
                 handle_Stag(ip, op, count, finp);
                 break;
              case 'I':	/* basic space, indent, fontsize tags		*/
                 handle_Itag(ip, op, count, finp);
                 break;
              case 'T':	/* reg=1, bold=2, italic=3, bold=4		*/
                 handle_Ttag(ip, op, count, finp);
                 break;
              case 'Q':	/* starts newline in table Jan26 p3769		*/
			/* maybe a justification style			*/
                 handle_Qtag(ip, op, count, finp);
                 break;
              case 'Z':	/* starts GPO employee sign-on header		*/
			/* terminated by 007 S|F			*/
                 handle_007Z(ip, op, count, finp);
                 break;
              case 'A':	/* force a page break				*/
                 (void)fprintf(op, "\n<!-- PJG %04o clearpage -->\n", next);
                 *count = 0;
                 break;
              case 'G':	/* inline special symbols followed by 1-8	*/
			/* or 256...257 usually terminated by KTAG	*/
                 handle_Gtag(ip, op, count, finp);
                 break;
              case 'K':	/* terminates special sym control seq		*/
                 handle_Ktag(ip, op, count, finp);
                 break;
              case 'L':	/* pseudotable ....... justification		*/
			/* p3995 Jan 28 r28ja3.od-cv no borders		*/
			/* not seen in a table environment		*/
                 if (typsetags[cTAG].status)
                    (void)fprintf(stderr,
                       "007 L encountered inside table file %s\n", finp);
                 (void)fprintf(op, "\n<!-- PJG %04o ellipses -->\n", next);
                 *count = 0;
                 break;

              case 'N':	/* indicates footnote ref or definition		*/
                 break;

              case 'c':	/* starts a table environment			*/
                 handle_starttable(ip, op, count, finp);
                 break;
              case 'e':	/* ends a table environment			*/
                 handle_endtable(ip, op, count, finp);
                 break;

              case 'a':	/* three times in Jan. unknown function		*/
                 (void)fprintf(op, "\n<!-- PJG 007 %04o unknown -->\n", next);
                 *count = 0;
                 break;

              case 'g':	/* starts header for imported graphic in S4725	*/
                 bungetc(more = bfgetc(ip), ip);
                 if (more == 's' || more == ',')
                    handle_imports(ip, op, count, finp);
                 else
                    fprintf(op, "\n<!-- PJG 007g skipped -->\n");
                 break;

              case 'q':	/* identical to Q tag				*/
                 handle_Qtag(ip, op, count, finp);
                 break;

              case 'D':	/* col delimiter in table			*/
              case 'P':	/* jump field in table tab horz to next		*/
			/* one see bottom of p3769 Jan26 also		*/
			/* col3 p2512 jan14 newline in a column		*/
              case 'r':	/* horiz rule across few cols in table		*/
			/* environment, followed by comma delim		*/
              case 's':	/* Centering in table. p3700 jan26,		*/
			/* the state name subheadings			*/
              case 'x':	/* and x tag is "xl" right justify in		*/
			/* table environment see p3768 Jan26		*/
              case 'h':	/* col text heading delimiter in table?		*/
              case 'j':	/* horz ruling in table ?			*/
              case 'l':
              case 'o':
              case 'f':	/* ends a table text, starts caption?		*/
			/* table footnote, see * top col2 p2513		*/
			/* jan14 still in the table			*/
                 if (typsetags[cTAG].status)
                    fprintf(op, "\n<!-- PJG 007 %04o intable -->\n", next);
                 else
                    fprintf(op, "\n<!-- PJG 007 %04o extable -->\n", next);
                 *count = 0;
                 break;

              case 'z':	/* column flush, start new column, ?		*/
                 (void)fprintf(op, "\n<!-- PJG %04o columnbreak -->\n", next);
                 *count = 0;
                 break;

              case '0':	/* seems to be horizontal spacing see p1751	*/
			/* col2 90% between stars r12ja3.xxx		*/
                 (void)fprintf(op, "\n<!-- PJG %04o hspace -->\n", next),
                 *count = fprintf(op, "&blank;");
                 break;

              case '1':	/* occurs twice. once in r19se3.xxx line 789	*/
			/* typo. meant to have ^GI11 got ^G11 instead	*/
			/* oddly exactly the same error occurs in the	*/
			/* identical text in file r07no3.xxx		*/
              case '2':	/* same typo as above, they meant to key an	*/
			/* Itag but deleted the I so we get ^G24 	*/
                        /* but in r06oc0 they intended a ^GD not I	*/
              case '8': /* same deal r06oc3.xxx missed a I in an ^GI83	*/
              case '9': /* again in r12oc2.xxx missed a I in an ^GI91	*/
                        /* on a RIN number line				*/
		 /* we could						*/
                 /* botch a repair by inserting an I tag back into the	*/
                 /* file. I is more common than D but this whole nasti-	*/
                 /* ness occurs only 10 times in six months of 94 files	*/
                 /* so forget about it					*/
                 (void)fprintf(op, "\n<!-- PJG %04o gpodeletetag -->\n", next);
                 *count = 0;
                 break;

              case '5': /* force a new line, the typesetter might do it	*/
			/* anyway so this is rare.			*/
                 (void)fprintf(op, "\n<!-- PJG %04o forcenewline -->\n", next);
                 *count = 0;
                 break;

              case ' ':	/* once in r12ja2.xxx. typo does nothing	*/
                 break;	/* p1702 col1 10%				*/

              default:
                 (void)fprintf(stderr,
                    "unknown control 007 +%04o+ file %s\n", next, finp);
                 break;
           }
           break;

        case '&':
           *count += fprintf(op, "&amp;");
           break;

        case ' ':
           if (*count > 95) (void)fprintf(op, "\n"), *count = 0;
           else             (void)fprintf(op, " ");
           break;

        default:
           /* if we are not about to accent this character and it is 	*/
           /* printable then go ahead. the test for upcoming accenting	*/
           /* is massively expensive					*/
           if (isprint(this))
           {
              bungetc(next = bfgetc(ip), ip);
              if (next != 0377 || !scan_soon(ip, "377 101 105", 3, 3))
                 *count += fprintf(op, "%c", this);
           }
           else
           {
              if (typsetags[ITAG].grid >= 5)
                 (void)fprintf(op, "\n<!-- PJG %04o grid5678 -->\n", this),
                 *count = 0;
              else
                 (void)fprintf(stderr,
                 "nonprinting noncontrol char +%04o+ file %s\n", this, finp);
           }
           break;
     }
     prev = this;
  }

  bungetc(this, ip);
  if (stop == STOP_BFEOF)
  {
     close_tags(op, &typsetags[ITAG], 2, count);
     close_tags(op, semantags, NSEMTAGS, count);
     close_tags(op, typsetags, NTYPTAGS, count);
     close_tags(op, dlimitags, NDOCTAGS, count);
  }
  return stop;
}


char *procargs(int argc, char **argv)
{
   if (argc != 2) 
      fatalerr("Usage", argv[0], "gpofile.xxx");
   return strdup(argv[1]);
}



/* the input string filename must be of the form r06ja0.xxx		*/
/* 06 = day, ja = month, 0 = X = the part of the federal register	*/
/* X can be 0 2 or 3.							*/
/* the output string of the form FR940106-0-12345			*/
/* FR94 is present for all 1994 docs. 01 is obtained from ja. -0- is	*/
/* of the form -Y- where Y is obtained from X thus Y = f(X)		*/
/* X = 0 ==> Y = 0	Rules and Regulations				*/
/* X = 2 ==> Y = 1	Proposed Rules					*/
/* X = 3 ==> Y = 2	Notices, Sunshine Act, Corrections etc		*/
/* this renumbering is unfortunate as the X number indicates which part	*/

static int XYfunc(char *);
static char *MMfunc(char *);

extern char *setdocstring(char *finp)
{
char day[3], month[3], num[2];

   if (finp[0] != 'r')
      fatalerr("setdocstring", finp, "does not begin with r");

   day[0] = finp[1];
   day[1] = finp[2];
   day[2] = '\0';
   month[0] = finp[3];
   month[1] = finp[4];
   month[2] = '\0';
   num[0] = finp[5];
   num[1] = '\0';

   sprintf(docstring, "FR94%s%s-%d", MMfunc(month), day, XYfunc(num));
   return strdup(docstring);
}

static int XYfunc(char *filenum)
{
   if (!strcmp(filenum, "0")) return 0; else
   if (!strcmp(filenum, "2")) return 1; else
   if (!strcmp(filenum, "3")) return 2; else
   fprintf(stderr, "docstring: XYfunc: unknown X %s\n", filenum);
}

static char *MMfunc(char *month)
{
   if (!strcmp(month, "ja")) return "01"; else
   if (!strcmp(month, "fe")) return "02"; else
   if (!strcmp(month, "mr")) return "03"; else
   if (!strcmp(month, "ap")) return "04"; else
   if (!strcmp(month, "my")) return "05"; else
   if (!strcmp(month, "jn")) return "06"; else
   if (!strcmp(month, "jy")) return "07"; else
   if (!strcmp(month, "au")) return "08"; else
   if (!strcmp(month, "se")) return "09"; else
   if (!strcmp(month, "oc")) return "10"; else
   if (!strcmp(month, "no")) return "11"; else
   if (!strcmp(month, "de")) return "12"; else
   fatalerr("MMfunc", month, "has no look up equivalent");
}
