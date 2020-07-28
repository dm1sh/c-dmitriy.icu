#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>

void err_msg(char *msg);
char *concat_to_front(char **str1, char *str2);
char *get_status_message(int status_code);
char *to_lower(char *str);
char *trim(char *str);
char *repair_spaces(char *str);
ssize_t get_dir_list(char ***dir_list, char *path);

#endif