#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
int main()
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(53000);
    serv.sin_addr.s_addr = inet_addr("10.120.10.105");

    char buffer[256] = "1111";
    // socklen_t l = sizeof(client);
    socklen_t m = sizeof(serv);
    //socklen_t m = client;
    // cout << "\ngoing to send\n";
    // cout << "\npls enter the mssg to be sent\n";
    // fgets(buffer, 256, stdin);
    int count = 0;
    while (1)
    {
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv, m);
        cout << "Message Sent!" << ++count << endl;
        usleep(1000);
    }
    //  recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&client,&l);
}