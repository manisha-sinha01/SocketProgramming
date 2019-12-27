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

int main(int argc, char* argv[])
{
    int tcp_fd;
    struct sockaddr_in serv_addr;
    char buf[BUFFERSIZE];
    char* TCPmsg = "Hi I am TCP";

    if((tcp_fd = socket(AF_INET, SOCK_STREAM,0)) < 0)
    {
        perror("TCP stream error");
        exit(EXIT_FAILURE);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(tcp_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    memset(buf, 0, BUFFERSIZE);
    write(tcp_fd,TCPmsg,strlen(TCPmsg));
    printf("Message form server:");
    int n = read(tcp_fd,buf,BUFFERSIZE);
    buf[n]='\0';
    printf("%s\n",buf);
    close(tcp_fd);

    return 0;
}