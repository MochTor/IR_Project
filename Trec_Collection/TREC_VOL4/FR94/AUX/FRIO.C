
#include <stdio.h>

#define INITBUF  524288
#define INCRBUF  131072
static char *buf;
static int position, maxbuf, numbuf;

int bio(char *finp)
{
int c;
FILE *ip;

   if ((ip = fopen(finp, "r")) == NULL)
      syserr("io_read", "fopen", finp);

   numbuf = 0;
   maxbuf = INITBUF;
   if ((buf = (char *)malloc(maxbuf * sizeof(char))) == NULL)
      syserr("io_read", "malloc", "space for buffer");

   for ( c = fgetc(ip) ; !feof(ip) ; c = fgetc(ip) )
   {
      buf[numbuf++] = (char)c;
      if (numbuf >= maxbuf)
         if ((buf = (char *)realloc(buf,
            (maxbuf += INCRBUF) * sizeof(char))) == NULL)
               syserr("io_read", "realloc", "bumped buffer space");
   }

   maxbuf = numbuf;
   if ((buf = (char *)realloc(buf, maxbuf * sizeof(char))) == NULL)
      syserr("io_read", "realloc", "final tailored buffer space");
   fclose(ip);
   position = 0;
   return 0;
}


int bfgetc(FILE *dummy)
{
   return (int)buf[position++];
}

int bungetc(int c, FILE *dummy)
{
   position--;
   return (position < 0 || position >= numbuf) ? -1 : c;
}

int bfeof(FILE *dummy)
{
   return position >= numbuf;
}
