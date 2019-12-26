#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define PORT 8080

int main(int argc, char* argv[]){
    
    int serv_fd, cli_fd;
    struct sockaddr_in serv_sock, cli_sock;
    char *mssgSrv = "Hi I am server";
    char buf[1024];

    if((serv_fd = socket(AF_INET, SOCK_DGRAM,0 )) <= 0)
    {
        perror("server error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_sock , 0, sizeof(serv_sock));
    memset(&cli_sock, 0 , sizeof(cli_sock));

    serv_sock.sin_family = AF_INET;
    serv_sock.sin_addr.s_addr = INADDR_ANY;
    serv_sock.sin_port = htons(PORT);

    if(bind(serv_fd, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) < 0)
    {
        perror("socket binding error");
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(cli_sock);
    n = recvfrom(serv_fd, (char*)buf, 1024, MSG_WAITALL, (struct sockaddr*)&cli_sock, &len);
    buf[n]='\0';
    printf("Server:%s\n",buf);

    sendto(serv_fd, (char*)mssgSrv, strlen(mssgSrv), MSG_CONFIRM, (struct sockaddr*)&cli_sock, len);
    printf("Message sent from server\n");

    
    return 0;


}