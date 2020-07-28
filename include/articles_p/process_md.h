#ifndef _PROCESS_MD_H
#define _PROCESS_MD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _ARTICLE_INFO
#define _ARTICLE_INFO

typedef struct
{
    char *title;
    long time;
    char *content;
    unsigned long length;
} article_info;

#endif

int process_md(article_info article, char **out);

#endif