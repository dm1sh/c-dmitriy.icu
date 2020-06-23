#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

/**
 * @brief Add str2 in front of str1
 * 
 * @param str1 
 * @param str2 
 * @return char* 
 */
char *add_to_front(char **str1, char *str2)
{
    char *tmp = strdup(*str1);

    strcpy(*str1, str2);
    strcat(*str1, tmp);

    free(tmp);

    return *str1;
}

/**
 * @brief Get the status message object
 * 
 * @param status_code 
 * @return char* 
 */
char *get_status_message(int status_code)
{
    switch (status_code)
    {
    case 200:
        return "OK";
    case 400:
        return "Internal Server Error";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "NOT FOUND";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    default:
        return "None";
        break;
    }
}

char *to_lower(char *str)
{
    for (char *p = str; *p != '\0'; p++)
        *p = tolower(*p);

    return str;
}