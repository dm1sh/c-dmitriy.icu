#ifndef _MIME_H
#define _MIME_H

#include <string.h>

#define DEFAULT_MIME_TYPE "application/octet-stream"

char *get_mime_type(char *file_path);

#endif