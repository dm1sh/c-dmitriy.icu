#include <stdio.h>

#include "../include/utils.h"

/** 
 * @brief Prints error
 * 
 * @param {char *} msg
*/
void err_msg(char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
}

char *add_to_front(char **str1, char *str2)
{
    char *tmp = strdup(*str1);
    
    strcpy(*str1, str2);
    strcat(*str1, tmp);

    free(tmp);

    return *str1;
}