#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>

#include "../include/netw.h"
#include "../include/utils.h"
/**
  * Main
  */
int main(int argc, char *argv[])
{
    int client_fd;
    struct sockaddr_storage cli_addr;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        err_msg("Usage: <Server Port>\n");
        exit(1);
    }
    char *port = argv[1];

    int listenfd = get_listener_socket(port);
    if (listenfd < 0)
    {
        err_msg("!fatal! couldn't create listening socket");
        exit(1);
    }
    printf("Waiting for connection on port %s...\n", port);

    while (1)
    {
        socklen_t sin_size = sizeof cli_addr;

        client_fd = accept(listenfd, (struct sockaddr *)&cli_addr, &sin_size);
        if (client_fd < 0)
        {
            err_msg("couldn't accept client connection");
            continue;
        }
        
        // | Doesn't work properly yet
        // v

        int pid = fork();
        
        if (pid < 0)
            err_msg("fork failed");
        else if (pid == 0)
        {
            close(listenfd);
        
            inet_ntop(cli_addr.ss_family, get_in_addr((struct sockaddr *)&cli_addr), s, sizeof s);
            printf("Got connection from %s\n", s);
        
            // connection(client_fd); // Implement
            exit(0);
        }
        else
            close(client_fd);
    }

    return 0;
}