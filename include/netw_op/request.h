#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

struct header_s {
    char * str;
    size_t size;
};

char *get_path(char *request);
struct header_s *gen_header(int status_code, size_t file_size, char *mime_type);
int send_response(int fd, char *req_path);
void handle_post_request(int fd, char *request);
void handle_get_request(int fd, char *request);