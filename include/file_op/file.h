#ifndef _FILE_H
#define _FILE_H

#include <string.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct file_s
{
    char *path;
    off_t size;
    int fd;
};

char *gen_file_path(char *path);
int send_file(int cli_fd, struct file_s *file);
struct file_s *get_file_info(char *file_path);
size_t get_file_size(FILE *file);

#endif