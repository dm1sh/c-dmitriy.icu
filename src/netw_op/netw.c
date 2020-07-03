#include "../../include/netw_op/netw.h"
#include "../../include/utils_op/utils.h"

/**
 * @brief Get the listener socket object
 * 
 * @param {char *} port 
 * @return int 
 */
int get_listener_socket(char *port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &servinfo) != 0)
    {
        err_msg("can't get addres info");
        return -1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            err_msg("can't create socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) < 0)
        {
            err_msg("can't set socket options");
            close(sockfd);
            freeaddrinfo(servinfo);
            return -2;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(sockfd);
            err_msg("can't bind socket");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        err_msg("failed to find local adress");
        return -3;
    }

    if (listen(sockfd, BACKLOG) < 0)
    {
        err_msg("failed to listen");
        return -4;
    }

    return sockfd;
}

/** 
 * @brief Get address from sockaddr structure
 * 
 * @param {struct sockaddr*} sa
 * @return void*
*/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}