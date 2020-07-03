#include "../../include/file_op/mime.h"
#include "../../include/utils_op/utils.h"

/**
 * @brief Get the mime type of file
 * 
 * @param file_path 
 * @return char* 
 */
char *get_mime_type(char *file_path)
{
    char *ext = strchr(file_path, '.');

    if (ext == NULL)
    {
        return DEFAULT_MIME_TYPE;
    }

    ext++;
    ext = to_lower(ext);

    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0)
    {
        return "text/html";
    }
    if (strcmp(ext, "jpeg") == 0 || strcmp(ext, "jpg") == 0)
    {
        return "image/jpg";
    }
    if (strcmp(ext, "css") == 0)
    {
        return "text/css";
    }
    if (strcmp(ext, "js") == 0)
    {
        return "application/javascript";
    }
    if (strcmp(ext, "json") == 0)
    {
        return "application/json";
    }
    if (strcmp(ext, "txt") == 0)
    {
        return "text/plain";
    }
    if (strcmp(ext, "gif") == 0)
    {
        return "image/gif";
    }
    if (strcmp(ext, "png") == 0)
    {
        return "image/png";
    }
    if (strcmp(ext, "ico") == 0)
    {
        return "image/x-icon";
    }

    return DEFAULT_MIME_TYPE;
}
