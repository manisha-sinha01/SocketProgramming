#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 8080

int main(int argc, char* argv[])
{
    int serv_fd;
    struct sockaddr_in serv_sock;
    char *mssgCl = "Hi I am client";
    char buf[1024];

    if((serv_fd = socket(AF_INET, SOCK_DGRAM,0)) <= 0)
    {
        perror("client error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_sock,0, sizeof(serv_sock));
    serv_sock.sin_addr.s_addr = INADDR_ANY;
    serv_sock.sin_family = AF_INET;
    serv_sock.sin_port = htons(PORT);

    sendto(serv_fd, (char*)mssgCl, strlen(mssgCl), MSG_CONFIRM, (const struct sockaddr*)&serv_sock,sizeof(serv_sock));
    printf("Client: Message sent\n");

    int len = sizeof(serv_sock);
    int n = recvfrom(serv_fd, (char*)buf, 1024, MSG_WAITALL,  (struct sockaddr*)&serv_sock,&len);
    buf[n]='\0';

    printf("Client :%s\n",buf);
    close(serv_fd);
    return 0;
}