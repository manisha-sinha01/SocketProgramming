#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#define BUFFERSIZE 1024
#define PORT 8080
using namespace std;

int main()
{
    int client,n;
    struct sockaddr_in serv_fd;
    char buf[1024];
    bool exitLoop = false;

    if((client = socket(AF_INET, SOCK_STREAM,0)) < 0)
    {
       // perror("socket error");
        exit(EXIT_FAILURE);
    }

    serv_fd.sin_family = AF_INET;
    serv_fd.sin_addr.s_addr = htons(INADDR_ANY);
    serv_fd.sin_port = htons(PORT);

    if(connect(client,(struct sockaddr*)&serv_fd,sizeof(serv_fd)) < 0)
    {
        //perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    cout << "\n Waiting for the confirmation from server" << endl;
    recv(client,buf,BUFFERSIZE,0);
    cout << "message from server :" << buf << endl;

    do
    {
        cout << "Client: " ;
        do{
            cin >> buf;
            n =send(client,buf,BUFFERSIZE,0);
            if(*buf == '#')
            {
                *buf = '*';
                exitLoop= true;    
            }

        }while(*buf != '*');
        cout << endl;
        
        do{
            recv(client,buf,BUFFERSIZE,0);
            cout << buf<<endl ;
            if(*buf == '#')
            {
                *buf ='*';
                exitLoop = true;
            }
        }while(*buf != '*');

        cout << endl;
    }while (!exitLoop);
    
    cout << "Process is terminated " << endl;
    close(client);

    return 0;
}