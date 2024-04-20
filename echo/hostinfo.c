#include "csapp.h"

int main(int argc, char **argv)
{
    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;

    if (argc != 2){
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_STREAM; // connection only
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    flags = NI_NUMERICHOST;
    for (p = listp; p; p = p->ai_next) {
        struct sockaddr_in *s = (struct sockaddr_in *)(p->ai_addr);
        if (inet_ntop(p->ai_family, &(s->sin_addr), buf, MAXLINE) != NULL) {
            printf("%s\n", buf);
        } else {
            fprintf(stderr, "inet_ntop error\n");
    }

    Freeaddrinfo(listp);
    exit(0);
    }
}