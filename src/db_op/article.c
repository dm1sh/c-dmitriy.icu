#include "../../include/db_op/db.h"

int expand_line_article(int n, char *buff, entry_s *rec)
{
#define entry rec->data.a

    entry.id = n;

    char *tmp = strtok(buff, ";");
    if (tmp == NULL)
        return -1;
    
    entry.header = malloc(strlen(tmp) + 1);
    entry.header = tmp;

    tmp = strtok(NULL, ";");
    if (tmp == NULL)
        return -2;
    
    entry.content = malloc(strlen(tmp) + 1);
    entry.content = tmp;

    tmp = strtok(NULL, ";");
    if (tmp == NULL)
        return -3;
    
    entry.author = malloc(strlen(tmp) + 1);
    entry.author = tmp;

    tmp = strtok(NULL, "\n");
    if (tmp == NULL)
        return -4;
    
    entry.topic = malloc(strlen(tmp) + 1);
    entry.topic = tmp;

    return 0;
}

char *serialize_article(entry_s *rec)
{
    int line_length = snprintf(NULL, 0, "%s;%s;%s;%s\n", entry.header, entry.content, entry.author, entry.topic) + 1;
    char *line = malloc(line_length);
    if (line == NULL)
        return "";

    snprintf(line, line_length, "%s;%s;%s;%s\n", entry.header, entry.content, entry.author, entry.topic);
    return line;
}
