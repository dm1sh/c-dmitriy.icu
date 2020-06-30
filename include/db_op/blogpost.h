#include <stdio.h>
#include <unistd.h>

typedef struct
{
    char *header;
    char *content;
    char **tags;
    char *author;
    unsigned int key;
} blogpost_t;