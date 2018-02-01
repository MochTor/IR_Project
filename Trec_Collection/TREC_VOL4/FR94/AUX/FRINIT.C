#include <stdio.h>
#include "frparse.h"
#include "frproto.h"

extern TAGS typsetags[NTYPTAGS] =
   {{"\n<!-- PJG FTAG %4d -->\n","\n<!-- PJG /FTAG -->\n", 0, 0, 0, 0, 0},
    {"\n<!-- PJG STAG %4d -->\n","\n<!-- PJG /STAG -->\n", 0, 0, 0, 0, 0},
    {"\n<FOOTCITE>",		"</FOOTCITE>\n",	0, 0, 0, 0, 0},
    {"\n<FOOTNOTE>",		"</FOOTNOTE>\n",	0, 0, 0, 0, 0},
    {"\n<TABLE>\n",		"\n</TABLE>\n",		0, 0, 0, 0, 0},
    {"\n<IMPORT>\n",		"\n</IMPORT>\n",	0, 0, 0, 0, 0},
    {"\n<!-- PJG ITAG l=%02d g=%1d f=%1d -->\n",
				"\n<!-- PJG /ITAG -->\n", 0, 1, 1, 0, 0},
    {"\n<!-- PJG QTAG %02d -->","\n<!-- PJG /QTAG -->\n", 0, 0, 0, 0, 0}};

extern TAGS dlimitags[] =
   {{"<DOC>\n",			"</DOC>\n\n\n\n",	0, 0, 0, 0, 0},
    {"<DOCNO>",			"</DOCNO>\n",		0, 0, 0, 0, 0},
    {"<PARENT>",		"</PARENT>\n", 		0, 0, 0, 0, 0},
    {"<TEXT>\n",		"</TEXT>\n",		0, 0, 0, 0, 0}};

/* these tags are mutually exclusive; i.e. when a tag shows up on the 	*/
/* stream, it closes all the others					*/
extern TAGS semantags[] =
   {{"\n<USDEPT>",	"</USDEPT>\n"	,	0, 0, 0, 0, 0},
    {"\n<CFRNO>",	"</CFRNO>",		0, 0, 0, 0, 0},
    {"\n<RINDOCK>",	"</RINDOCK>",		0, 0, 0, 0, 0},
    {"\n<DOCTITLE>",	"</DOCTITLE>",		0, 0, 0, 0, 0},
    {"\n<AGENCY>",	"</AGENCY>",		0, 0, 0, 0, 0},
    {"\n<ACTION>",	"</ACTION>",		0, 0, 0, 0, 0},
    {"\n<SUMMARY>",	"</SUMMARY>",		0, 0, 0, 0, 0},
    {"\n<DATE>",	"</DATE>",		0, 0, 0, 0, 0},
    {"\n<ADDRESS>",	"</ADDRESS>",		0, 0, 0, 0, 0},
    {"\n<FURTHER>",	"</FURTHER>",		0, 0, 0, 0, 0},
    {"\n<SUPPLEM>",	"</SUPPLEM>",		0, 0, 0, 0, 0},
    {"\n<SIGNER>",	"</SIGNER>",		0, 0, 0, 0, 0},
    {"\n<SIGNJOB>",	"</SIGNJOB>",		0, 0, 0, 0, 0},
    {"\n<FRFILING>",	"</FRFILING>",		0, 0, 0, 0, 0},
    {"\n<BILLING>",	"</BILLING>\n",		0, 0, 0, 0, 0},
    {"\n<USBUREAU>",	"</USBUREAU>",		0, 0, 0, 0, 0}};

extern char docstring[] = "";
