#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../include/file_op/file.h"

#define LINE_LENGTH 512;

typedef struct
{
    char *title;
    long time;
    char *content;
    unsigned long length;
} article_info;

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

int list_articles(article_info **articles)
{
    // FILE *file = fopen("./static/articles_list.db", "r");
    FILE *file = fopen("./test.db", "r");
    if (file == NULL)
    {
        perror("Couldn't open file");
        return -1;
    }

    char *buff = NULL;
    size_t length = LINE_LENGTH;
    size_t articles_amount = 0;

    while (getline(&buff, &length, file) > 0)
    {
        if (buff[strlen(buff) - 1] == '\n')
            buff[strlen(buff) - 1] = '\0';

        if (strlen(buff) == 0)
            continue;

        articles_amount++;

        article_info *tmp_articles = realloc(*articles, articles_amount * sizeof(article_info));
        if (tmp_articles == NULL)
            continue;
        *articles = tmp_articles;

        char *tmp = strdup(buff), *rest = tmp;

        char *article_time_str = strtok_r(tmp, " ", &rest);
        long article_time = strtol(article_time_str, NULL, 10);

        if (article_time == 0)
            continue;

        (*articles)[articles_amount - 1].time = article_time;

        rest = trim(rest);

        if (strlen(rest) > 0)
            (*articles)[articles_amount - 1].title = strdup(rest);
        else
            (*articles)[articles_amount - 1].title = "No title";

        printf("%ld - \"%s\"\n", (*articles)[articles_amount - 1].time, (*articles)[articles_amount - 1].title);
    }

    free(buff);

    return articles_amount;
}

long get_article_contents(article_info *article)
{
    char *name = strdup(article->title);
    if (strcmp(article->title, "No title") == 0)
    {
        int line_length = snprintf(NULL, 0, "%ld", article->time) + 1;
        name = malloc(line_length);
        if (name == NULL)
        {
            article->content = "500";
            return -1;
        }

        snprintf(name, line_length, "%ld", article->time);
    }

    // snprintf(NULL, 0, "./static/articles/%s/%s.md", article->title, article->title);
    int line_length = snprintf(NULL, 0, "./articles/%s/%s.md", article->title, article->title) + 1;
    char *path = malloc(line_length);
    if (path == NULL)
    {
        article->content = "500";
        return -1;
    }

    // snprintf(path, line_length, "./static/articles/%s/%s.md", article->title, article->title);
    snprintf(path, line_length, "./articles/%s/%s.md", article->title, article->title);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Couldn't open file");
        article->content = "404";
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    article->length = ftell(file);
    rewind(file);

    article->content = malloc(article->length + 1);

    for (int i = 0; i < article->length; i++)
    {
        article->content[i] = fgetc(file);
    }

    article->content[article->length] = '\0';

    return article->length;
}

/* Only for testing purposes */
int main()
{
    article_info *articles = malloc(0);

    int n = list_articles(&articles);

    printf("Read %d lines\n", n);
    printf("Last article's creation date is %ld and title is %s\n", articles[n - 1].time, articles[n - 1].title);

    get_article_contents(&(articles[0]));

    return 0;
}