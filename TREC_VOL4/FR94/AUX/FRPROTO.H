extern int  frparse(FILE *, FILE *, int *, char *, int f(FILE *, int));

extern void handle_007Z(FILE *, FILE *, int *, char *);
extern void handle_Ttag(FILE *, FILE *, int *, char *);
extern void handle_Itag(FILE *, FILE *, int *, char *);
extern void handle_Gtag(FILE *, FILE *, int *, char *);
extern void handle_Ktag(FILE *, FILE *, int *, char *);
extern void handle_Ptag(FILE *, FILE *, int *, char *);
extern void handle_Stag(FILE *, FILE *, int *, char *);
extern void handle_Ftag(FILE *, FILE *, int *, char *);
extern void handle_Qtag(FILE *, FILE *, int *, char *);
extern void handle_starttable(FILE *, FILE *, int *, char *);
extern void handle_endtable(  FILE *, FILE *, int *, char *);
extern void handle_footcite(  FILE *, FILE *, int *, char *);
extern void handle_last(      FILE *, FILE *, int *, char *);
extern void handle_256seq(FILE *);


extern void close_atag(FILE *, TAGS *, int *);
extern void close_tags(FILE *, TAGS *, int, int *);
extern void open_atag(FILE *, TAGS *, int *, int);
extern void open_itag(FILE *, TAGS *, int *, int, int, int);

extern char *procargs(int, char **);

extern TAGS typsetags[];
extern TAGS dlimitags[];
extern TAGS usdeptags[];
extern TAGS semantags[];

/* global variable used in the DOC line as used by Natalie. it	*/
/* needs to be dimensioned to at least				*/
/* strlen("<DOCNO> FR940106-2-12345 </DOCNO>");			*/

extern char docstring[40];
extern char *setdocstring(char *);
extern void echo_docno(FILE *, TAGS *, int *);
extern void echo_parent(FILE *, TAGS *, int *);
extern void bump_docno(void);
extern void bump_parent(void);
