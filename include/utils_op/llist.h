#ifndef _LLIST_H
#define _LLIST_H

#include <stdio.h>
#include <stdlib.h>

#define NUMBER 5

typedef struct llist_s
{
    char *value;
    struct llist_s *next;
} llist_t;

llist_t *find_item(llist_t *list, int n);
void fill_with_arr(llist_t **list, char **arr, size_t n);
void print_llist(llist_t *list);
llist_t *add_to_list(llist_t **head, int pos, char *value);

#endif