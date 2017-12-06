#include <stdio.h>
#include "frstop.h"

stop_bfeof(FILE *ip, int this)
{
   if (BFEOF) return STOP_BFEOF; 
   return 0;
}

int stop_billing(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (BFEOF)	return STOP_BFEOF;
   if (GPOEOF)	return STOP_GPOEOF;
   if (NEWLINE)	return STOP_NEWLINE;
   if (ZTERM)	return STOP_ZTERM;
   if (STERM)	return STOP_STERM;
   return 0;
}

int stop_dept(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (BFEOF)	return STOP_BFEOF;
   if (ITERM)	return STOP_ITERM;
   if (NEWLINE)	return STOP_NEWLINE;
   return 0;
}

int stop_cfr(FILE *ip, int this)
{
   return stop_dept(ip, this);
}

int stop_doctitle(FILE *ip, int this)
{
   return stop_dept(ip, this);
}

int stop_rindocket(FILE *ip, int this)
{
   return stop_dept(ip, this);
}

int stop_bureau(FILE *ip, int this)
{
   return stop_dept(ip, this);
}

int stop_graphic(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (BFEOF)	return STOP_BFEOF;
   if (ATERM)	return STOP_ATERM;
   if (TAYBLE)	return STOP_TAYBLE;
   if (ZTERM)	return STOP_ZTERM;
   if (STERM)	return STOP_STERM;
   return 0;
}

int stop_importformat(FILE *ip, int this)
{
   return stop_tableformat(ip, this);
}

int stop_tableformat(FILE *ip, int this)  /* should really look for I95	*/
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (NEWLINE)	return STOP_NEWLINE;
   if (BFEOF)	return STOP_BFEOF;
   if (LOCATOR)	return STOP_LOCATOR;
   if (ITERM)	return STOP_ITERM;
   return 0;
}


int stop_Gspecial(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (NEWLINE)	return STOP_NEWLINE;
   if (BFEOF)	return STOP_BFEOF;
   if (GTERM)	return STOP_GTERM;
   if (KTERM)	return STOP_KTERM;
   if (ZTERM)	return STOP_ZTERM;
   if (ITERM)	return STOP_ITERM;
   if (YDOTDOT)	return STOP_YDOTDOT;
   return 0;
}


int stop_footnote(FILE *ip, int this)
{
int that, two, thr;
   if (BFEOF)	return STOP_BFEOF;
   that = bfgetc(ip);	two = bfgetc(ip);	thr = bfgetc(ip);
   bungetc(thr, ip);	bungetc(two, ip);	bungetc(that, ip);
   if (BFEOF)	return STOP_BFEOF;
   if (NTERM)	return STOP_NTERM;
   if (TAYBLE)	return STOP_TAYBLE;
   if (STERM)	return STOP_STERM;
   if (ZTERM)	return STOP_ZTERM;
   if (ITERM &&	(two != '2' || thr != '8'))
		return STOP_ITERM;
   return 0;
}

int stop_footcite(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (BFEOF)	return STOP_BFEOF;
   if (NEWLINE)	return STOP_NEWLINE;
   if (NTERM)	return STOP_NTERM;
   if (ZTERM)	return STOP_ZTERM;
   return 0;
}

int stop_footlabel(FILE *ip, int this)
{
int that;
   if (BFEOF)	return STOP_BFEOF;
   bungetc((that = bfgetc(ip)), ip);
   if (BFEOF)	return STOP_BFEOF;
   if (KTERM)	return STOP_KTERM;
   if (YDOTDOT)	return STOP_YDOTDOT;
   return 0;
}
