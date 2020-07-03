#ifndef _ARTICLE_H
#define _ARTICLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/db_op/db.h"

#ifndef ARTICLE_T_TYPE
#define ARTICLE_T_TYPE

typedef struct {
    unsigned int id;
    char *header;
    char *content;
    char *author;
    char *topic;
} article_t;

#endif

int expand_line_article(int n, char *buff, entry_s *rec);
char *serialize_article(entry_s *rec);

#endif 