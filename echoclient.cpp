#include <iostream>
#include "unp.h"
void strcli(FILE* fp,int sockfd);
void strcli_select(FILE *fp,int sockfd);

int main(int argc,char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc!=2)
        err_quit("usage: echoclient <IPAddress>");

    sockfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8888);
    Inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

    Connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
    // strcli(stdin,sockfd);
    strcli_select(stdin,sockfd);
    exit(0);
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

void strcli_select(FILE *fp,int sockfd)
{
    int maxfdp1;
    fd_set rset;
    char sendline[MAXLINE],recvline[MAXLINE];

    FD_ZERO(&rset);
    for (;;)
    {
        FD_SET(fileno(fp),&rset);
        FD_SET(sockfd,&rset);
        maxfdp1=max(fileno(fp),sockfd)+1;
        Select(maxfdp1,&rset,nullptr,nullptr,nullptr);

        if (FD_ISSET(fileno(fp),&rset))
        {
            if (Fgets(sendline,MAXLINE,fp)==nullptr)
                return ;
            Writen(sockfd,sendline,strlen(sendline));
        }
        
        if (FD_ISSET(sockfd,&rset))
        {
            if (Readline(sockfd,recvline,MAXLINE)==0)
                err_quit("str_cli:server terminated prematurely");
            std::cout<<"receive msg : ";
            Fputs(recvline,stdout);
            // std::cout<<recvline<<std::endl;
        }

    }

}