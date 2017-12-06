
#include <stdio.h>
#include "frparse.h"
#include "frproto.h"
#include "frstop.h"

void handle_footcite(FILE *ip, FILE *op, int *count, char *finp)
{
   open_atag(op, &typsetags[FOOT], count, 1);
   frparse(ip, op, count, finp, stop_footcite);
   close_atag(op, &typsetags[FOOT], count);
}
