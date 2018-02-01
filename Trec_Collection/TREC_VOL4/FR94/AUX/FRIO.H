int bfgetc(FILE *);
int bungetc(int, FILE *);
int bfeof(FILE *);
int bio(char *);

extern int scan_tagint(FILE *, int);
extern int scan_immed(FILE *, char *, int);
extern int scan_post(FILE *, char *, int, int);
extern int scan_soon(FILE *, char *, int, int);
extern int scan_to(FILE *, char *, int, int);
extern void scan_trash(FILE *, int);
