#include "../../include/articles_op/html.h"
#include "../../include/articles_op/article.h"

int gen_html_article(article_info article, char **out)
{
    FILE *template = fopen("./static/articles/index.html", "r");
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

    int line_length = snprintf(NULL, 0, template_str, article.title, article.title, content) + 1;
    *out = malloc(line_length);
    if (*out == NULL)
    {
        *out = "500";
        return -1;
    }

    snprintf(*out, line_length, template_str, article.title, article.title, content);

    free(template_str);

    return line_length;
}

int gen_html_article_list(article_info *articles, int n, char **out)
{
    if (n == 0)
    {
        *out = "No articles found";
        return 0;
    }

    *out = strdup("<ul>\n");
    char *insert;

    for (int i = 0; i < n; i++)
    {
        int line_length = snprintf(NULL, 0, "<li><a href=\"/article/%d\" >%s</a></li>\n", i, articles[i].title) + 1;

        insert = malloc(line_length);
        *out = realloc(*out, strlen(*out) + line_length);
        if (insert == NULL || *out == NULL)
        {
            *out = "500 Memory error";
            return -1;
        }

        snprintf(insert, line_length, "<li><a href=\"/article/%d\" >%s</a></li>\n", i, articles[i].title);

        strcat(*out, insert);
    }

    *out = realloc(*out, strlen(*out) + strlen("</ul>") + 1);
    strcat(*out, "</ul>");

    free(insert);

    return 0;
}

// /* Only for testing purposes */
// int main ()
// {
//     article_info *articles = malloc(0);

//     int n = list_articles(&articles);

//     printf("Read %d lines\n", n);
//     printf("Last article's creation date is %ld and title is %s\n", articles[n - 1].time, articles[n - 1].title);

//     for (int i = 0; i < n; i++)
//         get_article_contents(&(articles[i]));

//     printf("Got content of %d files\n", n);

//     char *html;
//     gen_html_article_list(articles, n, &html);

//     printf("-%s-\n", html);

//     return 0;
// }