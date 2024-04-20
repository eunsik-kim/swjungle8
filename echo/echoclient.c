#include "csapp.h"

int main(int argc, char **argv)
{   
    // declaration
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3){
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port); // make client socket and connect to server
    Rio_readinitb(&rio, clientfd);  // init rio

    // until EOF, get string and printf buf
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        Rio_writen(clientfd, buf, strlen(buf)); // write to buf of clientfd
        Rio_readlineb(&rio, buf, MAXLINE); // read a line from buffer
        Fputs(buf, stdout);
    }
    
    Close(clientfd);    // already clientfd closed just for explicit closure
    exit(0);
}