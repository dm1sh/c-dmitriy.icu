#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../include/request.h"
#include "../include/utils.h"

/**
 * @brief Get the status message object
 * 
 * @param status_code 
 * @return char* 
 */
char *get_status_message(int status_code)
{
    switch (status_code)
    {
    case 200:
        return "OK";
    case 400:
        return "Internal Server Error";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "NOT FOUND";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    default:
        return "None";
        break;
    }
}

/**
 * @brief Get the path object
 * 
 * @param request 
 * @return char* 
 */
char *get_path(char *request)
{
    char *tmp = strtok(request, " ");
    char *path = strtok(NULL, " ");

    return path;
}

/**
 * @brief Generate header string
 * 
 * @param status_code 
 * @param file_size 
 * @param mine_type 
 * @return char* 
 */
char *gen_header(int status_code, size_t file_size, char *mine_type)
{
    char *status_message = get_status_message(status_code);

    time_t now = time(NULL);
    struct tm *ptm = gmtime(&now);
    char *curr_date = asctime(ptm);

    int header_length = snprintf(NULL, 0, "HTTP/1.1 %d %s\nDate: %s\nConnection: close\nContent-Length: %d\nContent-Type: %s\n\n", status_code, status_message, curr_date, file_size, mine_type);
    char *header = (char *)malloc(header_length + 1);
    snprintf(header, header_length + 1, "HTTP/1.1 %d %s\nDate: %s\nConnection: close\nContent-Length: %d\nContent-Type: %s\n\n", status_code, status_message, curr_date, file_size, mine_type);
    printf("%s\n", header);
}

/**
 * @brief Send HTTP response
 * 
 * @param fd 
 * @param file_path 
 * @return int 
 */
int send_response(int fd, char *path)
{
    int file_fd;
    struct stat stat_buf;
    file_fd = open(file_path, O_RDONLY);
    fstat(file_fd, &stat_buf);
    off_t f_size = stat_buf.st_size;
    off_t offset = 0;

    char *mine_type;

    char *header = gen_header(200, f_size, mine_type);

    sendfile(fd, file_fd, &offset, f_size);
    close(file_fd);
}

/**
 * @brief Handle POST request
 * 
 * @param fd 
 * @param request 
 */
void handle_post_request(int fd, char *request)
{
    char *not_supported_msg = "HTTP/1.1 501 Not Implemented\nConnection: close\n\n";
    send(fd, not_supported_msg, strlen(not_supported_msg), 0);
    printf("POST requests are not supported yet, closing connection\n");
}

/**
 * @brief Handle get request
 * 
 * @param fd 
 * @param request 
 */
void handle_get_request(int fd, char *request)
{
    char *path = get_path(request);
    printf("Client accessed path: %s\n", path);

    int rv = send_response(fd, path);
    if (rv < 0)
    {
        err_msg("couldn't send response");
    }
}