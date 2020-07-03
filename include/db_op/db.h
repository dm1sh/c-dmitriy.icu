#ifndef _DB_H
#define _DB_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

typedef enum
{
    BLOGPOST_T,
    ARTICLE_T
} types;

typedef struct
{
    types type;
    union {
        article_t a;
    } data;
} entry_s;

#include "./article.h"

#define LINE_SIZE 512

int open_db(char *file_name, types type, FILE **file, char ***head, int *head_length);
int open_table(char *file_name, FILE **file);
int read_head(FILE *file, char ***head);
int get_entry(FILE *file, entry_s *rec, int n, int (*process_line)(int, char *, entry_s *));
int append_table(FILE *file, entry_s *rec, char *(*process_line)(entry_s *));
int remove_entry(FILE *file, int n);
int read_whole_table(FILE *file, entry_s **rec, int (*process_line)(int, char *, entry_s *));
int find_by(FILE *file, entry_s **entries, int (*check_line)(int, char *, entry_s), int (*process_line)(int, char *, entry_s));

#endif