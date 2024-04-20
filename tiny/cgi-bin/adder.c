/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    char *buf, *p, *checkNULL;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;

    if ((buf = getenv("QUERY_STRING")) != NULL) {
        p = strchr(buf, '&');
        *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = strtol(arg1, &checkNULL, 10);
        if (*checkNULL != '\0')
            unix_error("strtol error");
        n2 = strtol(arg2, &checkNULL, 10);
        if (*checkNULL != '\0')
            unix_error("strtol error");
    }

    sprintf(content, "%s<p>Let me guess... The answer is : %d + %d = %d, right?\r\n", content, n1, n2, n1+n2);
    sprintf(content, "%s<p>Thanks for visiting!\r\n", content);

    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
/* $end adder */
