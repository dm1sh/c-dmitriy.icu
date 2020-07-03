#ifndef _ARR_H
#define _ARR_H

#include <string.h>
#include <stdlib.h>

void insert_to_arr(char ***arr, size_t length, char *value);
void free_arr(char **arr, int length);
int check_if_contains(char **arr, size_t length, char *str);

#endif