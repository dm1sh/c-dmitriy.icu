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

size_t get_file_size(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}

int list_articles(article_info **articles)
{
    // FILE *file = fopen("./static/articles_list.db", "r");
    FILE *file = fopen("./test_.db", "r");
    if (file == NULL)
    {
        perror("Couldn't open db file");
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
    int line_length = snprintf(NULL, 0, "./test_articles/%s/%s.md", name, name) + 1;
    char *path = malloc(line_length);
    if (path == NULL)
    {
        article->content = "500";
        return -1;
    }

    // snprintf(path, line_length, "./static/articles/%s/%s.md", article->title, article->title);
    snprintf(path, line_length, "./test_articles/%s/%s.md", name, name);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Couldn't open article file");
        article->content = "404";
        return -1;
    }

    article->length = get_file_size(file);

    article->content = malloc(article->length + 1);

    for (int i = 0; i < article->length; i++)
        article->content[i] = fgetc(file);

    article->content[article->length] = '\0';

    return article->length;
}

int process_md(article_info article, char **out)
{
    *out = malloc(0);
    char *tmp = strdup(article.content);
    char *buff = strtok(tmp, "\n");

    printf("%s\n", *out);

    int is_in_list = 0;

    while (buff != NULL)
    {
        if (strcmp(buff, "***") == 0)
        {
            char *tmp_out = realloc(*out, strlen(*out) + strlen("<hr>\n") + 1);
            if (tmp_out == NULL)
            {
                perror("Couldn't allocate memory for new element");
                continue;
            }
            *out = tmp_out;
            strcat(*out, "<hr>\n");
            continue;
        }
        if ((buff[0] == '*' && buff[1] == ' ') || (buff[0] == '-' && buff[1] == ' ') || (buff[0] == '+' && buff[1] == ' '))
        {
            char *begining = "";
            if (!is_in_list)
            {
                char *begining = "<ul>\n";
                is_in_list = 1;
            }

            memmove(buff, buff + 2, strlen(buff) - 1);

            size_t append_line_length = snprintf(NULL, 0, "%s<li>%s</li>\n", begining, buff) + 1;
            char *append = malloc(append_line_length);
            char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
            if (append == NULL || tmp_out == NULL)
            {
                perror("Couldn't allocate memory for new element");
                continue;
            }
            *out = tmp_out;

            snprintf(append, append_line_length, "%s<li>%s</li>\n", begining, buff);
            strcat(*out, append);

            free(append);
        }
        else
        {
            if (is_in_list)
            {
                char *tmp_out = realloc(*out, strlen(*out) + strlen("</ul>\n") + 1);
                if (tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;
                strcat(*out, "</ul>\n");
            }

            if (buff[0] == '#')
            {
                int n = 1;
                while (buff[n] == '#')
                    n++;

                memmove(buff, buff + n + 1, strlen(buff) - (n + 1) + 1); // n + 1 is to remove space after #

                size_t append_line_length = snprintf(NULL, 0, "<h%d>%s</h%d>\n", n, buff, n) + 1;
                char *append = malloc(append_line_length);
                char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                if (append == NULL || tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;

                snprintf(append, append_line_length, "<h%d>%s</h%d>\n", n, buff, n);
                strcat(*out, append);

                free(append);
            }
            else if (buff[0] == '>')
            {
                memmove(buff, buff + 2, strlen(buff) - 1);

                size_t append_line_length = snprintf(NULL, 0, "<blockquote>%s</blockquote>\n", buff) + 1;
                char *append = malloc(append_line_length);
                char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                if (append == NULL || tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;

                snprintf(append, append_line_length, "<blockquote>%s</blockquote>\n", buff);
                strcat(*out, append);

                free(append);
            }
            else
            {
                for (int i = 0; i < strlen(buff); i++)
                {
                    if (buff[i] == '*' && buff[i - 1] != '\\' && buff[i + 1] != ' ')
                    {
                        int n = 1;
                        while (buff[n] == '*')
                            n++;

                        char *tag = "";
                        if (n == 1)
                            tag = "i";
                        else if (n == 2)
                            tag = "b";
                        else if (n == 2)
                            tag = "mark";

                        int j = 0;
                        while (buff[i + j + n] != '*' && buff[i + j + n - 1] != '\\')
                            j++;

                        char *internal = malloc(j + 1);
                        memcpy(internal, buff + i + n, j);
                        internal[j] = '\0';

                        size_t append_line_length = snprintf(NULL, 0, "<%s>%s</%s>", tag, internal, tag) + 1;
                        char *append = malloc(append_line_length);
                        char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                        if (append == NULL || tmp_out == NULL)
                        {
                            perror("Couldn't allocate memory for new element");
                            continue;
                        }
                        *out = tmp_out;

                        snprintf(append, append_line_length, "<%s>%s</%s>", tag, internal, tag);
                        strcat(*out, append);

                        i += n * 2 + j;

                        free(append);
                        free(internal);
                    }
                    else
                    {
                        size_t len = strlen(*out);
                        char *tmp_out = realloc(*out, len + 2);
                        if (tmp_out == NULL)
                        {
                            perror("Couldn't allocate more memory to porcess Markdown");
                            return -1;
                        }
                        *out = tmp_out;
                        (*out)[len] = buff[i];
                        (*out)[len + 1] = '\0';
                    }
                }
            }
        }

        size_t len = strlen(*out);
        if ((*out)[len - 1] != '\n')
        {
            char *tmp_out = realloc(*out, len + 2);
            if (tmp_out == NULL)
            {
                perror("Couldn't allocate more memory to porcess Markdown");
                return -1;
            }
            *out = tmp_out;
            (*out)[len] = '\n';
            (*out)[len + 1] = '\0';
        }

        buff = strtok(NULL, "\n");
    }

    // printf("--%s--\n", *out);
    return strlen(*out);
}

int gen_html_article(article_info article, char **out)
{
    // FILE *template = fopen("./static/article.html", "r");
    FILE *template = fopen("./test_article.html", "r");
    if (template == NULL)
    {
        perror("Couldn't open article template");
        *out = "500";
        return -1;
    }

    size_t template_size = get_file_size(template);

    char *template_str = malloc(template_size + 1);

    for (int i = 0; i < template_size; i++)
        template_str[i] = fgetc(template);
    template_str[template_size] = '\0';

    *out = malloc(template_size + article.length + 1);

    char *content;

    process_md(article, &content);

    int line_length = snprintf(NULL, 0, template_str, article.title, content) + 1;
    *out = malloc(line_length);
    if (*out == NULL)
    {
        *out = "500";
        return -1;
    }

    snprintf(*out, line_length, template_str, article.title, content);

    return line_length;
}

/* Only for testing purposes */
int main()
{
    article_info *articles = malloc(0);

    int n = list_articles(&articles);

    printf("Read %d lines\n", n);
    printf("Last article's creation date is %ld and title is %s\n", articles[n - 1].time, articles[n - 1].title);

    for (int i = 0; i < n; i++)
        get_article_contents(&(articles[i]));

    printf("Got content of %d files\n", n);

    char **html = malloc(sizeof(char *) * n);
    for (int i = 0; i < n; i++)
        gen_html_article(articles[i], &(html[i]));

    return 0;
}