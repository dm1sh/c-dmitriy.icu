#include "../../include/netw_op/request.h"
#include "../../include/utils_op/utils.h"
#include "../../include/file_op/file.h"
#include "../../include/file_op/mime.h"
#include "../../include/articles_op/html.h"
#include "../../include/articles_op/article.h"

/**
 * @brief Send 404 response
 * 
 * @param fd 
 * @param path 
 */
void res_404(int fd, char *path)
{
    FILE *fp = fopen("static/404.html", "r");
    const ssize_t fsize = 512;
    char buff[fsize], msg[fsize];

    fread(buff, fsize, 1, fp);
    fclose(fp);

    sprintf(msg, buff, path, path);

    struct header_s *header = gen_header(404, strlen(msg), "text/html");
    send(fd, header->str, header->size - 1, 0);

    send(fd, msg, strlen(msg), 0);
    close(fd);

    printf("404 ERROR\n");
}

void res_500(int fd)
{
    char *msg = "Server error";

    struct header_s *header = gen_header(500, strlen(msg), "text/plain");
    send(fd, header->str, header->size - 1, 0);

    send(fd, msg, strlen(msg), 0);
    close(fd);

    printf("500 ERROR\n");
}

/**
 * @brief Get the path string
 * 
 * @param request 
 * @return char* 
 */
char *get_path(char *request)
{
    char *path = strtok(request, " ");

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
        res_500(fd);

        return 0;
    }

    if (file->fd < 0)
    {
        res_404(fd, req_path);

        return 0;
    }

    char *mime_type = get_mime_type(file_path);

    struct header_s *header = gen_header(200, file->size, mime_type);

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

    if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0)
    {
        FILE *fp = fopen("static/index.html", "r");
        if (fp == NULL)
        {
            res_404(fd, "/index.html");
            return;
        }

        size_t file_size = get_file_size(fp) + 1;
        char *template = malloc(file_size);

        fread(template, file_size, 1, fp);
        fclose(fp);

        article_info *articles = malloc(0);
        int amount = list_articles(&articles);
        if (amount < 0)
        {
            res_500(fd);
            return;
        }

        char *articles_list_str;
        gen_html_article_list(articles, amount, &articles_list_str);

        int line_length = snprintf(NULL, 0, template, articles_list_str) + 1;
        char *msg = malloc(line_length);
        if (msg == NULL)
        {
            res_500(fd);
            return;
        }

        snprintf(msg, line_length, template, articles_list_str);

        struct header_s *header = gen_header(200, strlen(msg), "text/html");
        send(fd, header->str, header->size - 1, 0);

        send(fd, msg, strlen(msg), 0);
        close(fd);

        printf("Sent home page\n");

        return;
    }

    if (strncmp(path, "/article/", strlen("/article/")) == 0)
    {
        char *id_str = (char *)malloc(strlen(path) - strlen("/article/") + 1);
        memmove(id_str, path + strlen("/article/"), strlen(path) - strlen("/article/") + 1);

        char *remaining;
        int id = strtol(id_str, &remaining, 10);
        if (id < 0 || strcmp(id_str, remaining) == 0)
        {
            res_404(fd, path);
            return;
        }

        article_info *articles = malloc(0);
        int amount = list_articles(&articles);
        if (amount < 0)
        {
            res_500(fd);
            return;
        }

        if (id > amount-1)
        {
            res_404(fd, path);
            return;
        }

        get_article_contents(&(articles[id]));

        char *html;
        gen_html_article(articles[id], &html);

        struct header_s *header = gen_header(200, strlen(html), "text/html");
        send(fd, header->str, header->size - 1, 0);

        send(fd, html, strlen(html), 0);
        close(fd);

        printf("Sent article with id=%d\n", id);

        free(articles);
        free(html);

        return;
    }

    if (send_response(fd, path) < 0)
    {
        err_msg("couldn't send response");
    }
}