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
    int udp_fd;
    struct sockaddr_in servaddr;
    char* msgUDP = "Hi I am UDP Client";
    char buf[BUFFERSIZE];

    
    if((udp_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        perror("udp client error");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    
    sendto(udp_fd,(const char*)msgUDP, strlen(msgUDP),0, (struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("UDP : Message sent to server\n");
    printf("message from server in UDP: ");
    int len = sizeof(servaddr);
    int n = recvfrom(udp_fd,(char*)buf,BUFFERSIZE,0,(struct sockaddr*)&servaddr,&len);
    buf[n]='\0';
    printf("%s\n",buf);
    
    return 0;
}