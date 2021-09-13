#include "../../include/articles_p/article.h"

int list_articles(article_info **articles)
{
    FILE *file = fopen("./static/articles/list.db", "r");
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
            (*articles)[articles_amount - 1].title = strdup("No title");

        (*articles)[articles_amount - 1].content = malloc(0);

        free(tmp);

        // printf("Found article \"%s\" posted on %ld\n", (*articles)[articles_amount - 1].time, (*articles)[articles_amount - 1].title);
    }

    fclose(file);

    free(buff);

    return articles_amount;
}

long get_article_contents(article_info *article)
{
    char *name = strdup(article->title);
    if (strcmp(article->title, "No title") == 0)
    {
        int line_length = snprintf(NULL, 0, "%ld", article->time) + 1;
        free(name);
        name = malloc(line_length);
        if (name == NULL)
        {
            article->content = strdup("500");
            return -1;
        }

        snprintf(name, line_length, "%ld", article->time);
    }

    int line_length = snprintf(NULL, 0, "./static/articles/%s/%s.md", name, name) + 1;
    char *path = malloc(line_length);
    if (path == NULL)
    {
        article->content = strdup("500");
        return -1;
    }

    snprintf(path, line_length, "./static/articles/%s/%s.md", name, name);

    FILE *file = fopen(path, "r");
    free(path);
    if (file == NULL)
    {
        perror("Couldn't open article file");
        article->content = strdup("404");
        return -1;
    }

    article->length = get_file_size(file);

    article->content = malloc(article->length + 1);

    for (int i = 0; i < article->length; i++)
        article->content[i] = fgetc(file);

    article->content[article->length] = '\0';

    free(name);
    fclose(file);

    return article->length;
}

void free_article_info_arr(article_info **articles, int length)
{
    for (int i = 0; i < length; i++)
    {
        free((*articles)[i].title);
        free((*articles)[i].content);
    }

    free(*articles);
}

// /* Only for testing purposes */
// int main()
// {
//     article_info *articles = malloc(0);

//     int n = list_articles(&articles);

//     printf("Read %d lines\n", n);
//     printf("Last article's creation date is %ld and title is %s\n", articles[n - 1].time, articles[n - 1].title);

//     for (int i = 0; i < n; i++)
//         get_article_contents(&(articles[i]));

//     printf("Got content of %d files\n", n);

//     char **html = malloc(sizeof(char *) * n);
//     for (int i = 0; i < n; i++)
//         gen_html_article(articles[i], &(html[i]));

//     return 0;
// }