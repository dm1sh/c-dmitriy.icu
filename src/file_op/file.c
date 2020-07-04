#include "../../include/file_op/file.h"
#include "../../include/utils_op/utils.h"

/**
 * @brief Generate file path from request path provided
 * 
 * @param req_path 
 * @return char* 
 */
char *gen_file_path(char *req_path)
{
    char *path = (char *)malloc(strlen(req_path) + 1);
    strcpy(path, req_path);
    if (strchr(req_path, '.') == NULL)
    {
        if (req_path[strlen(req_path) - 1] != '/')
        {
            path = realloc(path, strlen(path) + 1);
            path = strcat(path, "/");
        }
        path = realloc(path, strlen(path) + strlen("index.html") + 1);
        path = strcat(path, "index.html");
    }

    char *webroot = "static";

    path = realloc(path, strlen(path) + strlen(webroot));
    path = concat_to_front(&path, webroot);

    return path;
}

/**
 * @brief Send file to client
 * 
 * @param fd 
 * @param file_path 
 * @return int 
 */
int send_file(int cli_fd, struct file_s *file)
{
    off_t offset = 0;
    if (sendfile(cli_fd, file->fd, &offset, file->size + 2) < 0)
    {
        return -1;
    }

    close(file->fd);
    free(file);
    return 0;
}

/**
 * @brief Get the file info
 * 
 * @param file_path 
 * @return struct file_s* 
 */
struct file_s *get_file_info(char *file_path)
{
    struct file_s *file = (struct file_s *)malloc(sizeof(struct file_s));

    if (file == NULL)
    {
        err_msg("couldn't create file structure");
        return file;
    }

    struct stat stat_buf;

    file->fd = open(file_path, O_RDONLY);
    if (file->fd < 0)
    {
        return file;
    }

    fstat(file->fd, &stat_buf);
    file->size = stat_buf.st_size;

    return file;
}

/**
 * @brief Get the file size
 * 
 * @param file 
 * @return size_t 
 */
size_t get_file_size(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}