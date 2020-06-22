#include <string.h>
#include <stdlib.h>

#include "../include/file.h"
#include "../include/utils.h"

char *gen_file_path(char *req_path)
{
    char *path = (char *)malloc(sizeof req_path);
    strcpy(path, req_path);
    if (strchr(req_path, '.') == NULL)
    {
        if (req_path[strlen(req_path) - 1] != '/')
        {
            path = realloc(path, sizeof path + 1);
            path = strcat(path, "/");
        }
        path = realloc(path, sizeof path + sizeof "index.html");
        path = strcat(path, "index.html");
    }

    char *webroot = "../static";

    path = realloc(path, sizeof path + sizeof webroot);
    path = add_to_front(&path, webroot);

    return path;
}

int send_file(int fd, char *req_path)
{
    char *path = gen_file_path(req_path);

    
}