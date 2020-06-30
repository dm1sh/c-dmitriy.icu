#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "./blogpost.h"

#define LINE_SIZE 512

int open_db(char *file_name, char *type, FILE **file, char ***head, int *head_length);
int open_table(char *file_name, FILE **file);
int read_head(FILE *file, char ***head);
int get_entry(FILE *file, void **rec, int type, int n, int (*process_line)(char *, void **, int));
int append_table(FILE *file, void *rec, int type, char *(*process_line)(void *, int));
int remove_entry(FILE *file, int n);
