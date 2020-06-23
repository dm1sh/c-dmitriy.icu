struct file_s
{
    char *path;
    off_t size;
    int fd;
};

char *gen_file_path(char *path);
int send_file(int cli_fd, struct file_s *file);
struct file_s *get_file_info(char *file_path);