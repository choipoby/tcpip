#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <time.h>
#include <unistd.h>

char buff[1024];

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd<0) {
        printf("socket open error\n");
    }

    struct sockaddr_in serverAddr, clientAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(9993);

    // to send RST on close
    struct linger l;
    l.l_onoff = 1;
    l.l_linger = 0;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));


    int b = bind(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (b<0){
        printf("bind failed errno:%d", errno);
    }
    printf("bound\n");

    int lis = listen(fd, 1024);
    if (lis<0) {
        printf("listen failed errno:%d", errno);
    }
    printf("listening..\n");

    while(1){
        socklen_t len = sizeof(clientAddr);
        //int connfd = accept(fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        int connfd = accept(fd, (struct sockaddr *)&clientAddr, &len);
        if (connfd != 0) {
            printf("connection accepted\n");
            time_t ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            send(connfd, buff, strlen(buff), 0);
        } else {
            printf("not accepted errno:%d\n", errno);
        }
        close(connfd);
    }

}
