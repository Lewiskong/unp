#include "unp.h"
#include <iostream>

int main(int argc,char** args)
{
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;

	if (argc!=2) err_quit("usage: a.out <IPAddress>");
	if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) err_sys("socket error");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(13);

	if (inet_pton(AF_INET,args[1],&servaddr.sin_addr)<=0)
		err_quit("inet_pton error for %s",args[1]);
	if (connect(sockfd,(SA*)&servaddr,sizeof(servaddr))<0)
		err_sys("connect error");

	while ((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n]=0;
		if (fputs(recvline,stdout)==EOF)
			err_sys("fputs error");
	}
	if (n<0) err_sys("fputs error");
	exit(0);
}