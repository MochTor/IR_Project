#define NEWLINE		(this == 012)
#define STOP_NEWLINE	1
#define GPOEOF		(this == 032)
#define STOP_GPOEOF	2
#define YDOTDOT		(this == 0377)
#define STOP_YDOTDOT	3
#define LOCATOR		(this == 007)
#define STOP_LOCATOR	4
#define ATERM		(LOCATOR && that == 'A')
#define STOP_ATERM	5
#define ZTERM		(LOCATOR && that == 'Z')
#define STOP_ZTERM	6
#define STERM		(LOCATOR && that == 'S')
#define STOP_STERM	7
#define ITERM		(LOCATOR && that == 'I')
#define STOP_ITERM	8
#define KTERM		(LOCATOR && that == 'K')
#define STOP_KTERM	9
#define GTERM		(LOCATOR && that == 'G')
#define STOP_GTERM	10
#define NTERM		(LOCATOR && that == 'N')
#define STOP_NTERM	11
#define BFEOF		(bfeof(ip))
#define STOP_BFEOF	12
#define TAYBLE		(LOCATOR && that == 'c')
#define STOP_TAYBLE	13
#define G1T1		(scan_soon(ip, "007 107 061 007 124 061", 6, 6))
#define STOP_G1T1	14

extern int stop_bfeof(FILE *, int);
extern int stop_dept(FILE *, int);
extern int stop_billing(FILE *, int);
extern int stop_cfr(FILE *, int);
extern int stop_doctitle(FILE *, int);
extern int stop_rindocket(FILE *, int);
extern int stop_bureau(FILE *, int);
extern int stop_graphic(FILE *, int);
extern int stop_tableformat(FILE *, int);
extern int stop_importformat(FILE *, int);
extern int stop_Gspecial(FILE *, int);
extern int stop_footnote(FILE *, int);
extern int stop_footcite(FILE *, int);
extern int stop_footlabel(FILE *, int);
