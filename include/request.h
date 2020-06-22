char *get_path(char *request);
void gen_header(int status_code, size_t file_size, char *mine_type);
int send_response(int fd, char *path);
void handle_post_request(int fd, char *request);
void handle_get_request(int fd, char *request);