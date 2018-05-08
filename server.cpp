#include "unp.h"
#include <time.h>
#include <string>

int main(int argc,char** argv)
{
	int listenfd,connfd;
	struct sockaddr_in servaddr,clientaddr;
	socklen_t len;
	char buff[MAXLINE];

	time_t ticks;

	listenfd=Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(std::stoi(std::string(argv[1])));

	Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));

	Listen(listenfd,LISTENQ);

	for (;;)
	{
		// connfd=Accept(listenfd,(SA*)nullptr,nullptr);
		len=sizeof(clientaddr);
		connfd=Accept(listenfd,(SA*)&clientaddr,&len);
		ticks=time(nullptr);
		// snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		// snprintf(buff,sizeof(buff),"connection refused");
		snprintf(buff,sizeof(buff),"Connection from %s, port %d\n",
			Inet_ntop(AF_INET,&clientaddr.sin_addr,buff,sizeof(buff)),ntohs(clientaddr.sin_port));
		Write(connfd,buff,strlen(buff));

		Close(connfd);
	}
}