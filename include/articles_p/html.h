#ifndef _HTML_H
#define _HTML_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int gen_html_article(article_info article, char **out);
int gen_html_article_list(article_info *articles, int n, char **out);

#endif