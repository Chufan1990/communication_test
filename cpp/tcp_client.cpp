#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#define SERVPORT 54773
#define MAXDATASIZE 100
#define SERVER_IP "127.0.0.1"
#define DATA "2017"
/* 
 *自定义信息
 */
typedef struct MyMessage
{
    int ID;
    char info[256];
} MyMessage, *pMyMessage;
int main(int argc, char *argv[])
{
    int sockfd, recvbytes;
    //char buf[MAXDATASIZE];
    MyMessage recvData;
    struct sockaddr_in serv_addr;
    int counter = 0;
    while (1)
    {
        counter++;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket error!");
            exit(1);
        }
        bzero(&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(SERVPORT);
        serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("connect error!");
            exit(1);
        }

        // char buffer[256];
        // bzero(buffer, 256);

        // std::cin >> buffer;

        // std::cout << buffer << " " << strlen(buffer) << std::endl;
        // write(sockfd, buffer, strlen(buffer));

        write(sockfd, DATA, sizeof(DATA));

        memset((void *)&recvData, 0, sizeof(MyMessage));
        if ((recvbytes = recv(sockfd, (void *)&recvData, sizeof(MyMessage), 0)) == -1)
        {
            perror("recv error!");
            exit(1);
        }
        // buf[recvbytes] = '\0';
        printf("Received: ID=%d, Info=%s count=%d \n", recvData.ID, recvData.info, counter);

        close(sockfd);
        usleep(10000);
    }
    return 0;
}