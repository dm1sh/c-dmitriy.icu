#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "../include/request.h"
#include "../include/utils.h"
#include "../include/file.h"
#include "../include/mime.h"

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
 * @param mime_type 
 * @return char* 
 */
struct header_s *gen_header(int status_code, size_t file_size, char *mime_type)
{
    char *status_message = get_status_message(status_code);

    time_t now = time(NULL);
    struct tm *ptm = gmtime(&now);
    char *curr_date = asctime(ptm);

    int header_length = snprintf(NULL, 0, "HTTP/1.1 %d %s\nDate: %sConnection: close\nContent-Length: %ld\nContent-Type: %s\r\n\r\n", status_code, status_message, curr_date, file_size, mime_type) + 1;
    char *header_string = (char *)malloc(header_length);
    snprintf(header_string, header_length, "HTTP/1.1 %d %s\nDate: %sConnection: close\nContent-Length: %ld\nContent-Type: %s\r\n\r\n", status_code, status_message, curr_date, file_size, mime_type);

    struct header_s *header = (struct header_s *)malloc(header_length + sizeof(size_t));

    header->size = header_length;
    header->str = header_string;

    return header;
}

/**
 * @brief Send HTTP response
 * 
 * @param fd 
 * @param req_path 
 * @return int 
 */
int send_response(int fd, char *req_path)
{
    char *file_path = gen_file_path(req_path);

    struct file_s *file = get_file_info(file_path);
    if (file == NULL)
    {
        char *msg = "Server error";

        struct header_s *header = gen_header(500, sizeof(msg), "text/plain");
        send(fd, header->str, header->size - 1, 0);

        send(fd, msg, sizeof(msg), 0);
        close(fd);

        return 0;
    }
    if (file->fd < 0)
    {
        char *msg = "<html>\n<body>404, Not Found. Return to home? <a href=\"/\">Home</a></body><html/>";

        struct header_s *header = gen_header(404, strlen(msg), "text/html");
        send(fd, header->str, header->size - 1, 0);

        send(fd, msg, strlen(msg), 0);
        close(fd);

        return 0;
    }

    char *mime_type = get_mime_type(file_path);

    struct header_s *header = gen_header(200, file->size, mime_type);

    printf("\n---\n%s\n%ld\n---\n", header->str, header->size);

    int rv = send(fd, header->str, header->size - 1, 0);

    if (rv < 0)
    {
        err_msg("couldn't send header");
        return -1;
    }

    if (send_file(fd, file) < 0)
    {
        err_msg("couldn't send file");
        return -1;
    }

    close(fd);

    return 0;
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

    if (send_response(fd, path) < 0)
    {
        err_msg("couldn't send response");
    }
}