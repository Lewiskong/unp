#include <map>
#include "unp.h"

typedef void (*Handler)(int connfd);

// 点对点服务器（最多接收一个连接）
class Server
{
  public:
    static const int CONN_FD = -1;
    static Server New();

  public:
    Server();
    void Listen(int port);
    void Register(int fd, Handler handler);
    void Run();

  private:
    void process(int connfd);

  private:
    int listenfd;
    sockaddr_in servaddr;
    std::map<int, Handler> handlers;
};

Server Server::New()
{
    return Server();
}

Server::Server()
{
    this->listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = ntohl(INADDR_ANY);
}

void Server::Listen(int port)
{
    servaddr.sin_port = ntohs(port);
    Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    ::Listen(listenfd, LISTENQ);
}

void Server::Register(int fd, Handler handler)
{
    handlers[fd] = handler;
}

void Server::Run()
{
    sockaddr_in cliaddr;
    for (;;)
    {
        socklen_t clilen = sizeof(cliaddr);
        auto connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
        process(connfd);
        Close(connfd);
    }
}

void Server::process(int fd)
{
    fd_set rset;
    int maxfd = fd + 1;
    while (true)
    {
        if (handlers.find(CONN_FD) != handlers.end())
        {
            FD_SET(fd, &rset);
        }
        // add select fds
        for (auto it = handlers.cbegin(); it != handlers.cend(); it++)
        {
            if (it->first == CONN_FD)
                continue;
            FD_SET(it->first, &rset);
            if (it->first > maxfd)
                maxfd = it->first;
        }

        Select(maxfd, &rset, nullptr, nullptr, nullptr);

        if (FD_ISSET(fd, &rset))
        {
            handlers.find(CONN_FD)->second(fd);
        }

        for (int index = 0; index < maxfd; index++)
        {
            // index设备可读，处理对应事件
            if (FD_ISSET(index, &rset))
            {
                handlers.find(index)->second(index);
            }
        }
    }
}
