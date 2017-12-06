#define PERPETUATE_NATALIE_SKIP_DOCUMENT
#define PERPETUATE_NATALIE_DOCUMENT_DELIMITATION

typedef struct tgs
{
   char *begin,		/* string to write at opening of tag		*/
        *end;		/* string to write at closing of tag		*/

   int status;		/* is the tag active				*/
   int grid;		/* only for ITAG				*/
   int typeface;	/* only for ITAG				*/
   int statusminus1;
   int statusminus2;
} TAGS;

/* ----------------------------------- typsetting info tags -----------	*/

#define NTYPTAGS 8
#define FTAG 0		/* format					*/
#define STAG 1		/* subformat					*/
#define FOOT 2		/* footnote citation				*/
#define NTAG 3		/* 007 N defines a footnote cite and declare	*/
#define cTAG 4		/* 007 c opens a table, 007 e closes it.	*/
#define gTAG 5		/* opens graphics environment S4725 only	*/
#define ITAG 6		/* type - locator, gridtable and typeface	*/
#define QTAG 7		/* starts newline in table see Jan26 p3769	*/
                        /* maybe a justification style			*/

#define FTAG_LEN 4
#define STAG_LEN 4
#define ITAG_LEN 2
#define QTAG_LEN 2
#define FACE_LEN 1
#define GRID_LEN 1

/* ----------------------------------- document level tags ------------	*/
#define NDOCTAGS 4
#define DOC		0
#define DOCNO		1
#define PARENT		2
#define TEXT		3

/* ----------------------------------- field semantic tags ------------	*/
#define NSEMTAGS 16
#define DEPT		0
#define CFRNO		1
#define RINDOCKET	2
#define DOCTITLE	3
#define AGENCY		4
#define ACTION		5
#define SUMMARY		6
#define DATE		7
#define ADDRESS		8
#define FURTHER		9
#define SUPPLEM		10
#define SIGNATORY	11
#define SIGNATJOB	12
#define FRDOCFILING	13
#define BILLING		14
#define BUREAU 		15




/* ----------------------------------- additional codes ---------------	*/
#undef EXTRA_TABLE_STUFF
#define ATAG 0		/* force a page break 				*/
#define ZTAG 12		/* starts GPO employee sign-on header		*/
                        /* and 007 S 4700 ends it ??			*/
#define zTAG 26		/* column flush, start new column, ?		*/





#ifdef EXTRA_TABLE_STUFF

#define DTAG 1          /* column delimiter in table text               */
#define LTAG 6          /* tab in a table for justification see p3995   */
                        /* fri Jan 28 r28ja3.od-cv table has no borders */
#define PTAG 8          /* jump field in table tab horz to next one     */
                        /* see bottom of p3769 Jan26                    */
                        /* also col3 p2512 jan14 newline in a column    */

#define aTAG 13         /* three times in Jan. unknown function         */
#define fTAG 16         /* ends a table text, starts a caption ?        */
#define gTAG 17
#define hTAG 18         /* column text heading delimiter in table ?     */
#define jTAG 19         /* horz ruling in table ?                       */
#define lTAG 20
#define oTAG 21
#define qTAG 22
#define rTAG 23         /* horizontal rule across few cols in table     */
                        /* environment, always followed by n or s       */
#define sTAG 24         /* Centering in table. p3700 jan26, state name  */
#define xTAG 25         /* xl right justify in table environment        */
                        /* see p3768 Jan26                              */

#endif /* EXTRA_TABLE_STUFF */
