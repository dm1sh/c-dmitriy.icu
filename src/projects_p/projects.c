#include "../../include/projects_p/projects.h"
#include "../../include/utils_op/arr.h"
#include "../../include/file_op/file.h"

ssize_t read_list(project_t **list)
{
    FILE *fp = fopen("static/projects/list.db", "r");
    if (fp == NULL)
    {
        perror("couldn't open projectDB file");
        return -1;
    }

    char *buff = NULL;
    size_t length = LINE_LENGTH;
    size_t projects_amount = 0;

    while (getline(&buff, &length, fp) > 0)
    {
        if (buff[strlen(buff) - 1] == '\n')
            buff[strlen(buff) - 1] = '\0';

        if (strlen(buff) == 0)
            continue;

        projects_amount++;

        *list = realloc(*list, sizeof(project_t) * projects_amount);

        char *tmp = strdup(buff), *rest = tmp;

        (*list)[projects_amount - 1].title = strdup(strtok_r(tmp, ";", &rest));

        (*list)[projects_amount - 1].description = strdup(strtok_r(rest, ";", &rest));

        (*list)[projects_amount - 1].lang = strdup(strtok_r(rest, ";", &rest));

        (*list)[projects_amount - 1].license = strdup(strtok_r(rest, ";", &rest));

        (*list)[projects_amount - 1].url = strdup(strtok_r(rest, ";", &rest));

        free(tmp);
    }

    fclose(fp);

    free(buff);

    return projects_amount;
}

char *gen_project_html()
{
    FILE *template_fp = fopen("static/projects/item.html", "r");
    if (template_fp == NULL)
    {
        perror("couldn't open project tesplate");
        return NULL;
    }

    size_t template_file_size = get_file_size(template_fp) + 1;
    char *template = calloc(1, template_file_size);

    fread(template, template_file_size, 1, template_fp);
    fclose(template_fp);

    project_t *list = malloc(0);
    size_t length = read_list(&list);

    char *content = strdup("");

    for (size_t i = 0; i < length; i++)
    {
        int line_length = snprintf(NULL, 0, template, list[i].url, list[i].title, list[i].description, list[i].lang, list[i].license) + 1;
        char *project_content = calloc(1, line_length);

        sprintf(project_content, template, list[i].url, list[i].title, list[i].description, list[i].lang, list[i].license);

        content = realloc(content, strlen(content) + line_length);

        content = strcat(content, project_content);

        free(project_content);
    }

    free_proj_list(&list, length);
    free(template);

    return content;
}

void free_proj_list(project_t **list, int len)
{
    for (int i = 0; i < len; i++)
    {
        free((*list)[i].description);
        free((*list)[i].lang);
        free((*list)[i].license);
        free((*list)[i].title);
        free((*list)[i].url);
    }

    free(*list);
}