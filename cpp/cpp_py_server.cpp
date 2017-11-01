#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

using namespace std;

#define SERVER_PORT htons(54377)
#define PORT 54377
#define EXIT_FAILURE 1

#if 0
int main() {

        char buffer[1000];
        string str = "11111";
        int n;

        int serverSock=socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = SERVER_PORT;
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        /* bind (this socket, local address, address length)
           bind server socket (serverSock) to server address (serverAddr).  
           Necessary so that server can use a specific port */ 
        bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

        // wait for a client
        /* listen (this socket, request queue length) */
        listen(serverSock,1);

                // bzero(buffer, 1000);

                sockaddr_in clientAddr;
                socklen_t sin_size=sizeof(struct sockaddr_in);
                int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);
        while (1 == 1) {
                bzero(buffer, 1000);
                // int clientSock=accept(serverSock,(struct sockaddr*)&clientAddr, &sin_size);
                //receive a message from a client
                n = read(clientSock, buffer, 500);
                cout << "Confirmation code  " << n << endl;
                cout << "Server received:  " << buffer << endl;

                // strcpy(buffer, "1111");
                // n = write(clientSock, buffer, strlen(buffer));

                unsigned char buffer2[str.length()];
                copy(str.begin(),str.end(),buffer2);                                
                n = write(clientSock,buffer2,strlen(buffer2));
                
                cout << "Confirmation code  " << n << endl;
                // close(clientSock);
        }
        return 0;
}
#else
int main(int argc, char const *argv[])
{
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
        char *hello = "0-12341234\0";
        char fuck[256]; 

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
                perror("socket failed");
                return -1;
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
                perror("setsockopt");
                return -1;
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
                perror("bind failed");
                return -1;
        }
        if (listen(server_fd, 3) < 0)
        {
                perror("listen");
                return -1;
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
                perror("accept");
                return -1;
        }
        while(1)
        {
                valread = read(new_socket, buffer, 1024);
                printf("%s\n", buffer);
                // bcopy(hello,fuck,strlen(fuck));      
                send(new_socket, hello, strlen(hello), 0);
                printf("Message Sent\n");
        }
        return 0;
}
#endif