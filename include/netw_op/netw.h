#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

int get_listener_socket(char *port);
void *get_in_addr(struct sockaddr *sa);