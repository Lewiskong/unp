#include "unp.h"
#include <iostream>

void strecho(int connfd);
void sig_chld(int signo);

int main(int argc,char **args)
{
    int listenfd,connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;

    listenfd=Socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);
    Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));

    // process child process fin signal to close time_wait
    Listen(listenfd,LISTENQ);
    Signal(SIGCHLD,sig_chld);

    for ( ; ; )
    {
        clilen=sizeof(cliaddr);
        connfd=Accept(listenfd,(SA*)&cliaddr,&clilen);

        if (childpid=Fork()==0)
        {
            Close(listenfd);
            strecho(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }
}

void strecho(int connfd)
{
    long arg1,arg2;
    ssize_t n;
    char buf[MAXLINE];
    int result=0;
again:
    while (n=read(connfd,buf,MAXLINE)){
        // if ((result=sscanf(buf,"%ld,%ld",&arg1,&arg2))==2)
        //     snprintf(buf,sizeof(buf),"%ld\n",arg1+arg2);
        // std::cout<<result<<std::endl;
        // n=strlen(buf);
        Write(connfd,buf,n);
    }
    // 处理系统调用被中断
    if (n<0 && errno==EINTR)
        goto again;
    else if (n<0)
        err_sys("strecho: read error");
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    // while ((pid=waitpid(-1,&stat,WNOHANG))>0) 
    // {
    //     std::cout<<"child process "<<pid<<" terminated"<<std::endl;
    // }

    return ;
}