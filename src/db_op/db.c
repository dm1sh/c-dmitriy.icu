#include "../../include/db_op/db.h"
#include "../../include/utils_op/utils.h"
#include "../../include/utils_op/arr.h"

/**
 * @brief Opens a table of specified type. If no such table, creates one and returns 1
 * 
 * @param file_name 
 * @param type 
 * @param file 
 * @param head 
 * @param head_length 
 * @return int 
 */
int open_db(char *file_name, types type, FILE **file, char ***head, int *head_length)
{
    char *type_string;
    switch (type)
    {
    case BLOGPOST_T:
        type_string = "blogpost";
        break;

    case ARTICLE_T:
        type_string = "article";
        break;

    default:
        err_msg("This type is not supported");
        return -1;
    }

    FILE *root_file;
    if ((root_file = fopen(file_name, "a+")) == NULL)
    {
        err_msg("can't open database file");
        return -1;
    }

    char *buff = NULL;
    size_t size = LINE_SIZE;

    while (getline(&buff, &size, root_file) >= 0)
        if (strcmp(type_string, strtok(buff, " ")) == 0)
        {
            int ret = open_table(strtok(NULL, "\n"), file);

            *head_length = read_head(*file, head);

            fclose(root_file);

            return ret;
        }

    free(buff);

    struct stat st = {0};
    if (stat("./db", &st) == -1)
        mkdir("./db", 0700);

    int table_file_name_size = snprintf(NULL, 0, "./db/%s.csv", type_string) + 1;
    char *table_file_name = malloc(table_file_name_size);
    snprintf(table_file_name, table_file_name_size, "./db/%s.csv", type_string);

    fseek(root_file, 0, SEEK_SET);
    fprintf(root_file, "%s %s\n", type_string, table_file_name);

    fclose(root_file);

    int ret = open_table(table_file_name, file);

    free(table_file_name);

    return 1;
}

/**
 * @brief Open table file
 * 
 * @param file_name 
 * @param file 
 * @return int -1 if couldn't open file
 */
int open_table(char *file_name, FILE **file)
{
    if ((*file = fopen(file_name, "a+")) == NULL)
    {
        err_msg("can't open table file");
        return -1;
    }

    return 0;
}

/**
 * @brief Read table head
 * 
 * @param file 
 * @param head 
 * @return int 
 */
int read_head(FILE *file, char ***head)
{
    fseek(file, 0, SEEK_SET);

    int length = 0;
    *head = malloc(length * sizeof(char *));

    char buff[LINE_SIZE];
    fscanf(file, "%[^\n]", buff);

    char *tok;
    int i = 0;
    for (tok = strtok(buff, ";"); tok && *tok; tok = strtok(NULL, ";"))
        insert_to_arr(head, length++, tok);

    return length;
}

/**
 * @brief Get the entry on line n
 * 
 * @param file 
 * @param rec 
 * @param n 
 * @param process_line 
 * @return int 
 */
int get_entry(FILE *file, entry_s *rec, int n, int (*process_line)(int, char *, entry_s *))
{
    char *buff = NULL;
    int i = 0;
    size_t size = LINE_SIZE;

    fseek(file, 0, SEEK_SET);

    while (getline(&buff, &size, file) >= 0)
    {
        if (i == n)
        {
            int res = (*process_line)(n, buff, rec);
            return res;
        }
        else
            i++;
    }

    free(buff);

    return -1;
}

/**
 * @brief Insert entry to the end of table 
 * 
 * @param file 
 * @param rec 
 * @param process_line 
 * @return int 
 */
int append_table(FILE *file, entry_s *rec, char *(*process_line)(entry_s *))
{
    fseek(file, 0, SEEK_END);
    char *buff = (*process_line)(rec);

    if (fprintf(file, "%s", buff) < 0)
        return -1;

    free(buff);
    return 0;
}

/**
 * @brief Remove entry located at line n
 * 
 * @param file 
 * @param n 
 * @return int 
 */
int remove_entry(FILE *file, int n)
{
    if (n == 0)
    {
        err_msg("you shouldn't remove table head. Aborting");
        return -1;
    }

    int tmp_file_name_length = snprintf(NULL, 0, "./db/tmp%d.csv", n) + 1;
    char *tmp_file_name = malloc(tmp_file_name_length);
    snprintf(tmp_file_name, tmp_file_name_length, "./db/tmp%d.csv", n);

    FILE *tmp_file = fopen(tmp_file_name, "w");

    char *buff = NULL;
    size_t size = LINE_SIZE;

    int i = 0;

    fseek(file, 0, SEEK_SET);

    while (getline(&buff, &size, file) >= 0)
    {
        if (i != n)
            fprintf(tmp_file, "%s", buff);

        i++;
    }

    fclose(tmp_file);
    free(buff);

    int fd = fileno(file);
    char file_path[1024];

    int path_length = snprintf(NULL, 0, "/proc/self/fd/%d", fd) + 1;
    char *path = malloc(path_length);
    snprintf(path, path_length, "/proc/self/fd/%d", fd);

    memset(file_path, 0, sizeof(file_path));
    ssize_t l = readlink(path, file_path, sizeof(file_path) - 1);
    free(path);

    if (l < 0)
    {
        err_msg("couldn't get table path");
        return -1;
    }
    file_path[l] = '\0';

    if (remove(file_path) != 0)
    {
        err_msg("couldn't remove original file");
        return -1;
    }

    if (rename(tmp_file_name, file_path) != 0)
    {
        err_msg("couldn't rename tmp file");
        return -1;
    }
    free(tmp_file_name);

    return i;
}

int read_whole_table(FILE *file, entry_s **entries, int (*process_line)(int ,char *, entry_s *))
{
    char *buff;
    int i = 0;
    size_t size = LINE_SIZE;

    fseek(file, 0, SEEK_SET);

    printf("Begining line-by-line reading\n");

    while (getline(&buff, &size, file) >= 0)
    {

        if (i == 0)
        {
            i++;
            continue;
        }

        printf("Read line [%d]: %s\n", i, buff);

        int res = (*process_line)(i, buff, &((*entries)[i]));
        if (res < 0)
            return res;

        i++;

        printf("---%lu---\n", sizeof(entry_s));
        *entries = (entry_s *)realloc(*entries, sizeof(entry_s) * i);
    }
    if (i == 1)
        return -1;

    free(buff);

    return ++i;
}