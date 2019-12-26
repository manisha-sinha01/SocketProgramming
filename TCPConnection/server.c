#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#define PORT 8080
#define BUFFERSIZE 1024
#define SOCKET_ERROR 0

int main(int argc, char const* argv[])
{
    //server socket file description
    int server_fd, opt=1, client_fd;
    struct sockaddr_in servsocket;
    int socketaddr = sizeof(servsocket);
    char buf[BUFFERSIZE];
    char *message = "Server message";

    // Socket file descriptor for accessing the socket
    if((server_fd = socket(AF_INET,SOCK_STREAM,0)) == SOCKET_ERROR)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Attaching the socket to port we want, the attachment is forced so that
    // port not available messages are not displayed
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    servsocket.sin_family = AF_INET;
    servsocket.sin_addr.s_addr = INADDR_ANY;
    servsocket.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&servsocket, sizeof(servsocket))< 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,3)<0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    if((client_fd = accept(server_fd,(struct sockaddr*)&servsocket, (socklen_t*)&socketaddr))<0)
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    int size = read(client_fd, buf, BUFFERSIZE);
    printf("Client: %s\n",buf);

    send(client_fd, message, strlen(message),0);
    printf("Server: I have sent my message\n");

    return 0;
}