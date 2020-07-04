#include "../../include/articles_op/process_md.h"

int process_md(article_info article, char **out)
{
    *out = malloc(1);
    (*out)[0] = '\0';
    char *rest = strdup(article.content);
    char *buff;

    int is_in_list = 0;

    while ((buff = strtok_r(rest, "\n", &rest)) != NULL)
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
                begining = "<ul>\n";
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
            continue;
        }

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
            is_in_list = 0;
        }

        if (buff[0] == '#')
        {
            int n = 1;
            while (buff[n] == '#')
                n++;
            int k = (n > 6) ? 6 : n;

            memmove(buff, buff + n + 1, strlen(buff) - (n + 1) + 1); // n + 1 is to remove space after #

            size_t append_line_length = snprintf(NULL, 0, "<h%d>%s</h%d>\n", k, buff, k) + 1;
            char *append = malloc(append_line_length);
            char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
            if (append == NULL || tmp_out == NULL)
            {
                perror("Couldn't allocate memory for new element");
                continue;
            }
            *out = tmp_out;

            snprintf(append, append_line_length, "<h%d>%s</h%d>\n", k, buff, k);
            strcat(*out, append);

            free(append);
            continue;
        }

        if (buff[0] == '>')
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
            continue;
        }

        char *tmp_out = realloc(*out, strlen(*out) + strlen("<p>") + 1);
        if (tmp_out == NULL)
        {
            perror("Couldn't allocate memory for new element");
            continue;
        }
        *out = tmp_out;

        strcat(*out, "<p>");

        for (int i = 0; i < strlen(buff); i++)
        {
            if (buff[i] == '*' && buff[i - 1] != '\\' && buff[i + 1] != ' ')
            {
                int n = 1;
                while (buff[i + n] == '*')
                    n++;

                char *tag = "";
                if (n == 1)
                    tag = "i";
                else if (n == 2)
                    tag = "b";
                else if (n == 3)
                    tag = "mark";

                int j = 0;
                while (buff[i + j + n] != '*' && buff[i + j + n - 1] != '\\')
                    j++;

                char *internal = malloc(j + 1);
                memcpy(internal, buff + i + n, j);
                internal[j] = '\0';

                size_t append_line_length = snprintf(NULL, 0, "<%s>%s</%s> ", tag, internal, tag) + 1;
                char *append = malloc(append_line_length);
                char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                if (append == NULL || tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;

                snprintf(append, append_line_length, "<%s>%s</%s> ", tag, internal, tag);
                strcat(*out, append);

                i += n * 2 + j;

                free(append);
                free(internal);

                continue;
            }

            if (buff[i] == '!' && buff[i + 1] == '[')
            {
                int n = 0;
                while (buff[i + 2 + n] != ']')
                    n++;

                if (buff[i + 2 + n + 1] != '(')
                    continue;

                int k = 0;
                while (buff[i + 2 + n + 2 + k] != ')')
                    k++;

                char *internal_text = malloc(n + 1);
                memcpy(internal_text, buff + i + 2, n);
                internal_text[n] = '\0';

                char *src = malloc(k + 1);
                memcpy(src, buff + i + 2 + n + 2, k);
                src[k] = '\0';

                size_t append_line_length = snprintf(NULL, 0, "<img src=\"%s\" alt=\"%s\"> ", src, internal_text) + 1;
                char *append = malloc(append_line_length);
                char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                if (append == NULL || tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;

                snprintf(append, append_line_length, "<img src=\"%s\" alt=\"%s\"> ", src, internal_text);
                strcat(*out, append);

                i += 2 + n + 2 + k + 1;

                free(append);
                free(src);
                free(internal_text);

                continue;
            }

            if (buff[i] == '[')
            {
                int n = 0;
                while (buff[i + 1 + n] != ']')
                    n++;

                if (buff[i + 1 + n + 1] != '(')
                    continue;

                int k = 0;
                while (buff[i + 1 + n + 2 + k] != ')')
                    k++;

                char *internal_text = malloc(n + 1);
                memcpy(internal_text, buff + i + 1, n);
                internal_text[n] = '\0';

                char *href = malloc(k + 1);
                memcpy(href, buff + i + 1 + n + 2, k);
                href[k] = '\0';

                size_t append_line_length = snprintf(NULL, 0, "<a href=\"%s\">%s</a> ", href, internal_text) + 1;
                char *append = malloc(append_line_length);
                char *tmp_out = realloc(*out, strlen(*out) + append_line_length);
                if (append == NULL || tmp_out == NULL)
                {
                    perror("Couldn't allocate memory for new element");
                    continue;
                }
                *out = tmp_out;

                snprintf(append, append_line_length, "<a href=\"%s\">%s</a> ", href, internal_text);
                strcat(*out, append);

                i += 1 + n + 2 + k + 1;

                free(append);
                free(href);
                free(internal_text);

                printf("++%d-%c++\n", i, buff[i]);

                continue;
            }

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

            printf("**%d-%c**\n", i, buff[i]);
        }

        tmp_out = realloc(*out, strlen(*out) + strlen("</p>") + 1);
        if (tmp_out == NULL)
        {
            perror("Couldn't allocate memory for new element");
            continue;
        }
        *out = tmp_out;

        strcat(*out, "</p>");

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
    }
    (*out)[strlen(*out) - 1] = '\0';

    return strlen(*out);
}
