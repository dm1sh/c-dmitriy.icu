#include "../../include/utils_op/utils.h"

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
char *concat_to_front(char **str1, char *str2)
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

/**
 * @brief Remove unneded spaces at the begining and ending of string
 * 
 * @param str 
 * @return char* 
 */
char *trim(char *str)
{
    while (str[0] == ' ' || str[0] == '\n' || str[0] == '\t')
    {
        memmove(str, str + 1, strlen(str));
    }
    while (str[strlen(str) - 1] == ' ' || str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == '\t')
    {
        str[strlen(str) - 1] = '\0';
    }

    return str;
}

char *repair_spaces(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '_')
        {
            str[i] = ' ';
        }
    }

    return str;
}

/**
 * @brief Gets the list of files and directories at the specified path
 * 
 * @param dir_list 
 * @param path
 * @return ssize_t 
 */
ssize_t get_dir_list(char ***dir_list, char *path)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path)) == NULL)
    {
        perror("\nOpendir");
        return -1;
    }

    ssize_t n = 0;
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        n++;
        *dir_list = realloc(*dir_list, sizeof(char *) * n);
        (*dir_list)[n - 1] = strdup(ent->d_name);

        if (ent->d_type == DT_DIR)
        {
            (*dir_list)[n - 1] = realloc((*dir_list)[n - 1], strlen((*dir_list)[n - 1]) + 2);
            (*dir_list)[n - 1] = strcat((*dir_list)[n - 1], "/");
        }
    }

    closedir(dir);

    return n;
}