#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define MAXLINE 1024
char recvline[MAXLINE];
int main()
{
    // create a socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd<0) {
        printf("socket open error\n");
    }
    // construct an address to connect
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(9993);

    if ( connect(fd,  (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0 ) {
        printf("connect error\n");
    }

    int n;
    while ( ( n = recv(fd, recvline, MAXLINE, 0) ) > 0 ) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            printf("fputs error\n");
        }
    }

    if (n <= 0) {
        printf("n:%d, errno:%d\n", n, errno);
    }

    return 0;
}
