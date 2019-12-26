#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080

int main(int argc, char const* argv[]){

    int serv_fd, content;
    struct sockaddr_in serv_addr;
    char *messgCl = "This is the client side";
    char buffer[1024];

    if((serv_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("client error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0)
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    if(connect(serv_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("connection failed\n");
        exit(EXIT_FAILURE);
    }
    send(serv_fd, messgCl, strlen(messgCl),0);
    printf("Client : Message sent\n");
    content = read(serv_fd, buffer, 1024);
    printf("Client: %s\n",buffer);

    return 0;
}