#ifndef _PROJECTS_H
#define _PROJECTS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LINE_LENGTH 512

typedef struct {
    char *title;
    char *description;
    char *lang;
    char *license;
    char *url;
} project_t;

ssize_t read_list(project_t **list);
char *gen_project_html();
void free_proj_list(project_t **list, int len);

#endif