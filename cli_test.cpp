#include <iostream>
#include "unp.h"

void strcli(FILE* fp,int sockfd);

int main(int argc,char **argv) 
{
    int sockfd;
    struct sockaddr_in servAddr,clientAddr;

    sockfd=Socket(AF_INET,SOCK_STREAM,0);

    // set serv addr
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family=AF_INET;
    servAddr.sin_port=htons(SERV_PORT);
    Inet_pton(AF_INET,"127.0.0.1",&servAddr.sin_addr);

    // set client addr
    bzero(&clientAddr,sizeof(clientAddr));
    clientAddr.sin_family=AF_INET;
    clientAddr.sin_port=htons(44444);
    Inet_pton(AF_INET,"127.0.0.1",&clientAddr.sin_addr);

    // bind
    Bind(sockfd,(SA*)&clientAddr,sizeof(clientAddr));

    Connect(sockfd,(SA*)&servAddr,sizeof(servAddr));
    strcli(stdin,sockfd);
}

void strcli(FILE* fp,int sockfd)
{
    char sendline[MAXLINE],recvline[MAXLINE];
    while (Fgets(sendline,MAXLINE,fp)!=nullptr)
    {
        Writen(sockfd,sendline,strlen(sendline));
        if (Readline(sockfd,recvline,MAXLINE)==0)
            err_quit("strcli: server termiated prematuredly");
        Fputs(recvline,stdout);
    }
}