#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <cstring>

using namespace std;

#define PORT 54377
#define MAX_SIZE 1024
int sender(char *data);

int main()
{
        double a = 3.1415926;
        char data[1024] = {"0"};
        int n;
        while (1)
        {
                sprintf(data, "%2.5f", a);
                a *= 0.3;

                sender(data);
        }
        return 0;
}
int mysocket(char *data)
{
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char rbuffer[MAX_SIZE] = {0};
        char tbuffer[MAX_SIZE] = {0};

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
                perror("Socket Failed!");
                return -1;
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
                perror("Set Socket Opt Failed!");
                return -1;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port PORT
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
                perror("Bind Failed!");
                return -1;
        }

        if (listen(server_fd, 3) < 0)
        {
                perror("Listen");
                return -1;
        }

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
                perror("Accept Failed");
                return -1;
        }
        return 0;
}
        // Transferring data
int sender(int ){
        valread = read(new_socket, rbuffer, 1024);
        printf("%s\n", rbuffer);
        bcopy(tbuffer, data, strlen(data));
        send(new_socket, tbuffer, strlen(tbuffer), 0);
        printf("Message sent\n");

        return 0;
}