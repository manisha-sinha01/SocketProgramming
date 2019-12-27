#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <unistd.h> 
#define PORT 8080
#define BUFFERSIZE 1024

int max(int x, int y)
{
    return (x>y?x:y);
}

int main(int argc, char* argv[])
{
    int tcp_fd, udp_fd, max_fd,fd_ready, cli_fd;
    struct sockaddr_in serv_addr,cli_addr;
    char buf[BUFFERSIZE];
    int len,n;
    pid_t childpid;
    fd_set fds;
    char *servMsg = "Hi I am the server for TCP & UDP";
    
    if((tcp_fd = socket(AF_INET, SOCK_STREAM,0)) < 0)
    {
        perror("TCP stream error");
        exit(EXIT_FAILURE);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(tcp_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    listen(tcp_fd,3);

    if((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("UDP socket error");
        exit(EXIT_FAILURE);
    }

    bind(udp_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    FD_ZERO(&fds);
    max_fd = max(tcp_fd,udp_fd) + 1;

    while(1)
    {

        FD_SET(tcp_fd,&fds);
        FD_SET(udp_fd,&fds);

        fd_ready = select(max_fd,&fds,NULL,NULL,NULL);

        if(FD_ISSET(tcp_fd,&fds))
        {
            len = sizeof(cli_addr);
            cli_fd = accept(tcp_fd,(struct sockaddr*)&cli_addr,&len);
            if((childpid = fork()) == 0)
            {
                close(tcp_fd);
                bzero(buf,BUFFERSIZE);
                printf("Server: Hi I am TCP \n");
                read(cli_fd,buf,BUFFERSIZE);
                puts(buf);
                write(cli_fd,(const char*)servMsg,strlen(servMsg));
                close(cli_fd);
                exit(0);
            }
            close(cli_fd);
        }

        if(FD_ISSET(udp_fd,&fds))
        {
            len =sizeof(cli_addr);
            bzero(buf,BUFFERSIZE);
            printf("Server: Hi I am UDP \n");
            n= recvfrom(udp_fd,buf,BUFFERSIZE,0,(struct sockaddr*)&cli_addr,&len);
            buf[n]='\0';
            printf("Buffer from UDP:%s\n",buf);
            sendto(udp_fd,(const char*)servMsg,strlen(servMsg),0,(struct sockaddr*)&cli_addr,len);
            printf("server:message sent to udp\n");

        }

    }

    return 0;
}