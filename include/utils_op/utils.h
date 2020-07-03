#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void err_msg(char *msg);
char *concat_to_front(char **str1, char *str2);
char *get_status_message(int status_code);
char *to_lower(char *str);

#endif