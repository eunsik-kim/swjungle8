#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

typedef struct node_l node_l;
struct node_l {
    node_l *left;
    node_l *right;
    int responseSize;
    int headerSize;
    char *header;
    char *response;
};
typedef struct {
    node_l *head;
    node_l *tail;
    int size;
} linked_list;

int request_transfer(int clientfd, rio_t *rio);
int parse_header(int clientfd, char *hostname, char *method, char *uri);
void response_transfer(int clientfd, int serverfd, rio_t *rio);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
void *thread(void *vargp);

static linked_list *LList;  // shared variables

int main(int argc, char **argv) 
{
    int listenfd, *clientfd;
    struct sockaddr_storage clientaddr;
    char clienthost[MAXLINE], clientport[MAXLINE];
    socklen_t clientlen;
    pthread_t tid;

    LList = (linked_list *)malloc(sizeof(linked_list));
    LList->head = NULL;
    LList->tail = NULL;
    LList->size = 0;

    signal(SIGPIPE, SIG_IGN);
    /* checking proxy port args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    listenfd = Open_listenfd(argv[1]);
    int i = 0;
    while (1) {
        clientlen = sizeof(clientaddr);
        clientfd = (int *)Malloc(sizeof(int));
        *clientfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, clienthost, MAXLINE, clientport, MAXLINE, 0);
        printf("Accepted connection from client (%s, %s)\n", clienthost, clientport);
        Pthread_create(&tid, NULL, thread, clientfd);
        // Pthread_join(tid, NULL);     // if not detach, use join for memory leak
        printf("It's my %d circle.\n", ++i);
        printf("\n");
    }
    return 0;
}

void *thread(void *vargp)
{    
    Pthread_detach(pthread_self());
    printf("Hi, I'm %ld tid.\n", pthread_self());

    int serverfd, clientfd = *((int *)vargp);
    Free(vargp);

    rio_t rio;
    Rio_readinitb(&rio, clientfd);
    if ((serverfd = request_transfer(clientfd, &rio)) != -1)
        response_transfer(clientfd, serverfd, &rio);
    printf("====This connection is terminated====\n");
    Close(clientfd);
    return NULL;
}

/* parsing and fillin missing header then, transfer request to server */
int request_transfer(int clientfd, rio_t *rio)
{   
    char buf[MAXLINE], hostname[MAXLINE], con[40], proxcon[40];
    char method[10], uri[MAXLINE], version[10];
    int serverfd, isUserAgent=0, isHost=0, isCon=0, isProxCon=0;
    
    Rio_readlineb(rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    printf("Request uri: %s\n", uri);

    // cached data check
    node_l *startNode = LList->head;
    while (startNode != NULL){
        if ((startNode->headerSize == strlen(rio->rio_buf))
                && !memcmp(rio->rio_buf, startNode->header, startNode->headerSize)){
            rio_writen(clientfd, startNode->response, startNode->responseSize);
            printf("This request is responsed by cached Data\n");
            return -1;
        }
        startNode = startNode->right;
    }

    // only GET request
    if (strcasecmp(method, "GET")){ 
        clienterror(clientfd, method, "501", "Not implemented", "Tiny does not implement this method");
        return -1;
    }
    // ignore favicon request
    if (strstr(uri, "favicon")) {
        printf("Tiny couldn't find the favicon.\n");
        return -1;
    }

    strcpy(hostname, "Host: ");
    if ((serverfd = parse_header(clientfd, hostname, method, uri)) == -1)
        return -1;

    while(strcmp(buf, "\r\n")) {
        Rio_readlineb(rio, buf, MAXLINE);
        // check existance of important request headers
        if (strstr(buf, "Host"))
            isHost = 1;
        if (strstr(buf, "User-Agent"))
            isUserAgent = 1;

        if (strstr(buf, "Proxy-Connection"))
            isProxCon = 1;
        else if (strstr(buf, "Connection"))
            isCon = 1;

        if (strcmp(buf, "\r\n"))
        {
            printf("%s", buf);
            rio_writen(serverfd, buf, strlen(buf));
        }
    }

    // insert important missing headers
    if (!isHost){
        printf("%s", hostname);
        rio_writen(serverfd, hostname, strlen(user_agent_hdr));
    }
    if (!isUserAgent){
        printf("%s", user_agent_hdr);
        rio_writen(serverfd, (void *)user_agent_hdr, strlen(user_agent_hdr));
    }
    if (!isCon){
        strcpy(con, "Connection: close\r\n");
        printf("%s", con);
        rio_writen(serverfd, con, strlen(con));
    }
    if (!isProxCon){
        strcpy(proxcon, "Proxy-Connection: close\r\n");
        printf("%s", proxcon);
        rio_writen(serverfd, proxcon, strlen(proxcon));
    }
    rio_writen(serverfd, buf, strlen(buf)); // insert "\r\n" at the end of header 
    return serverfd;
}

int parse_header(int clientfd, char *hostname, char *method, char *uri)
{   
    int serverfd, ishttps = 0;
    char serverhost[MAXLINE], serverport[MAXLINE], new_uri[MAXLINE];

    // parsing header request
    char *hostptr, *portptr, *rootptr;
    hostptr = index(uri, ':') + 3;
    if (*(hostptr-4) == 's')
        ishttps = 1;    
    rootptr = index(hostptr, '/');

    // exception for html src request
    if (rootptr == NULL){
        clienterror(clientfd, "last of address '/' needed", "400", "Bad request", "Proxy have to add '/' at last address");
        return -1;
    }
    
    *rootptr = '\0';
    strcat(hostname, hostptr);
    strcat(hostname, "\r\n");
    portptr = index(hostptr, ':');
    
    if (portptr){
        strcpy(serverport, portptr + 1);
        *portptr = '\0';
    }
    else if(ishttps)    // https default port
        strcpy(serverport, "443");
    else                // http default port
        strcpy(serverport, "80");
    strcpy(serverhost, hostptr);

    // concatenate header request
    strcpy(new_uri, method);
    strcat(new_uri, " /");
    if (!strcmp(rootptr, "\0"))
        strcat(new_uri, rootptr+1);
    strcat(new_uri, " ");
    strcat(new_uri, "HTML/1.0\r\n");   // modify version HTML/1.1 into HTML/1.0

    serverfd = Open_clientfd((char *)serverhost, (char *)serverport);
    rio_writen(serverfd, new_uri, strlen(new_uri));
    return serverfd;
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];
    sprintf(body, "\r\n<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
    
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

void response_transfer(int clientfd, int serverfd, rio_t *rio)
{   
    char responseHeader[MAXLINE], *buf;
    int read_cnt=0, acc_cnt=0;
    int responseSize=0, responseHeaderSize=0;
    rio_t rp;
    // find Content-length
    Rio_readinitb(&rp, serverfd);
    do {
        buf = responseHeader + responseHeaderSize;
        read_cnt = Rio_readlineb(&rp, buf, MAXLINE);
        rio_writen(clientfd, buf, read_cnt);
        responseHeaderSize += read_cnt;
        if (strstr(buf, "Content-length"))
            responseSize = atoi(index(buf, ':') + 2);
    } while (strcmp(buf, "\r\n"));
    
    if (responseHeaderSize + responseSize <= MAX_OBJECT_SIZE)
    {   
        // initial Node to save resonse and request header(key)
        node_l *newNode = (node_l *)malloc(sizeof(node_l));
        newNode->header = (char *)malloc(strlen(rio->rio_buf));
        newNode->response = (char *)malloc(responseHeaderSize + responseSize);
        newNode->headerSize = strlen(rio->rio_buf);
        newNode->responseSize = responseHeaderSize + responseSize;
        // copy response header and body
        strncpy(newNode->header, rio->rio_buf, newNode->headerSize);
        strncpy(newNode->response, responseHeader, responseHeaderSize);
        buf = newNode->response + responseHeaderSize;
        while ((read_cnt = Rio_readnb(&rp, buf, responseSize)) > 0){
            rio_writen(clientfd, buf, read_cnt);
            if (errno == EPIPE)
                return;
            responseHeaderSize += read_cnt;
            buf = newNode->response + responseHeaderSize;
        }
        acc_cnt = responseSize;

        // insert the newNode into the tail of linkedlist by LRU
        newNode->right = NULL;
        newNode->left = LList->tail;
        if (LList->tail != NULL)
            newNode->left->right = newNode; 
        LList->tail = newNode;
        if (LList->head == NULL){
            LList->head = newNode;
        }
    }
    else
    {
        char responsebuf[responseSize];
        while ((read_cnt = Rio_readlineb(&rp, responsebuf, responseSize)) > 0){
            rio_writen(clientfd, responsebuf, read_cnt);
            if (errno == EPIPE)
                return;
            acc_cnt += read_cnt;
        }
    }
    printf("Response-length: %d\n", acc_cnt);
    Close(serverfd);
}