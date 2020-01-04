#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 8080
#define BUFFERSIZE 1024

using namespace std;

int main()
{
    int server, client;
    struct sockaddr_in server_fd;
    char buf[BUFFERSIZE];
    bool exitLoop = false;

    if((server = socket(AF_INET,SOCK_STREAM, 0)) < 0)
    {
        //perror("Server socket failed");
        cout << "server socket error" << endl;
        exit(EXIT_FAILURE);
    }

    server_fd.sin_family = AF_INET;
    server_fd.sin_addr.s_addr = htons(INADDR_ANY);
    server_fd.sin_port = htons(PORT);
    socklen_t size = sizeof(server_fd);

    if((bind(server, (struct sockaddr*)&server_fd, sizeof(server_fd))) < 0)
    {
        //perror("bind error");
        cout << "bind error" << endl;
        exit(EXIT_FAILURE);
    }

    listen(server, 1);

    if((client = accept(server,(struct sockaddr*)&server_fd, &size)) < 0)
    {
        //perror("accept error");
        cout << "accept error" << endl;
        exit(EXIT_FAILURE);
    }

    while(client > 0)
    {
        cout << "Ready to send confirmation to client" << endl;
        strcpy(buf,"hello client");
        send(client,buf,BUFFERSIZE,0);
        cout <<"confirmation sent " << endl;
        

        do{
            cout << "From client :";
            do{
                recv(client,buf,BUFFERSIZE,0);
                cout << buf << endl;
                if(*buf == '#')
                {
                 *buf = '*';
                  exitLoop = true;
                }
            }while(*buf != '*');

            //cout << "Server:";
            int n;
            do{
                
                cin >> buf;
                n=send(client,buf,BUFFERSIZE,0);
                if(*buf == '#')
                {
                    *buf = '*';
                    exitLoop = true;
                }

            }while(*buf != '*');

            
            
        }while(!exitLoop);

        cout << "Connection Terminated" << endl;
        exitLoop = false;
        close(client);
        exit(1);
    }

    close(server);

    return 0;
}