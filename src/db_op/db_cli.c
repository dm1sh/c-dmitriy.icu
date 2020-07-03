#include "../../include/db_op/db_cli.h"
#include "../../include/utils_op/arr.h"

char *get_all_articles_list()
{
    FILE *db_file;
    char **db_head;
    int length = -1;

    int ret = open_db("root.db", ARTICLE_T, &db_file, &db_head, &length);
    if (ret == 1)
    {
        perror("No such database\n");
    }

    entry_s *articles_table = (entry_s *)malloc(sizeof(entry_s));
    if (articles_table == NULL)
        return "";

    size_t entries_amount = read_whole_table(db_file, &articles_table, expand_line_article);

    printf("\nStarting processing table\n");

    size_t topics_amount = 0;
    char **articles_by_topics_lists = malloc(topics_amount * sizeof(char *));
    char **topics_list = malloc(topics_amount * sizeof(char));
    if (articles_by_topics_lists == NULL || topics_list == NULL)
        return "";

    printf("Initialized lists arrays\n");

    size_t topics_list_size = 1;

    for (int i = 0; i < entries_amount; i++)
    {
        printf("Processing entry [%d]\n", i);

        article_t rec = articles_table[i].data.a;

        int pos = check_if_contains(topics_list, topics_amount, rec.topic);
        if (pos == 0)
        {
            topics_amount++;

            char **tmp_topics_list = realloc(topics_list, topics_list_size + strlen(rec.topic));
            if (tmp_topics_list == NULL)
                continue;
            else
                topics_list = tmp_topics_list;

            topics_list_size += strlen(rec.topic);

            pos = topics_amount - 1;

            topics_list[pos] = rec.topic;

            char **tmp_articles_by_topics_lists = realloc(articles_by_topics_lists, topics_amount * sizeof(char *));
            if (tmp_articles_by_topics_lists == NULL)
                continue;
            else
                articles_by_topics_lists = tmp_articles_by_topics_lists;
        }

        int line_length = snprintf(NULL, 0, "<li>%s</li>\n", rec.header) + 1;
        char *tmp_line = malloc(line_length);
        if (tmp_line == NULL)
            return "";

        snprintf(tmp_line, line_length, "<li>%s</li>\n", rec.header);

        char *tmp_articles_by_topics_lists_line = realloc(articles_by_topics_lists[pos], strlen(articles_by_topics_lists[pos]) + line_length + 1);
        if (tmp_articles_by_topics_lists_line == NULL)
            continue;
        else
            articles_by_topics_lists[pos] = tmp_articles_by_topics_lists_line;

        strncat(articles_by_topics_lists[pos], tmp_line, line_length);
    }

    char *articles_table_list_str = malloc(topics_amount * strlen("<h1></h1>\n<ul></ul>\n"));
    if (articles_table_list_str == NULL)
        return "";

    for (int i = 0; i < topics_amount; i++)
    {
        int add_len = snprintf(NULL, 0, "<h1>%s</h1>\n<ul>%s</ul>\n", topics_list[i], articles_by_topics_lists[i]);
        char *tmp_line = malloc(add_len);
        if (tmp_line == NULL)
            continue;

        snprintf(tmp_line, add_len, "<h1>%s</h1>\n<ul>%s</ul>\n", topics_list[i], articles_by_topics_lists[i]);

        char *tmp_articles_table_list_str = realloc(articles_table_list_str, strlen(articles_table_list_str) + add_len + 1);
        if (tmp_articles_table_list_str == NULL)
            continue;
        else
            articles_table_list_str = tmp_articles_table_list_str;

        strcat(articles_table_list_str, tmp_line);
    }

    return articles_table_list_str;
}