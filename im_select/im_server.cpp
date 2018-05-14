#include <iostream>
#include "unp.h"
#include "server.hpp"

int main()
{
    auto server = Server::New();
    server.Listen(8888);
    server.Register(Server::CONN_FD, [](int fd) {
        char recvline[MAXLINE];
        if (Readline(fd, recvline, MAXLINE) == 0)
            err_quit("server quited permanently!");
        std::cout << "receive msg : " << recvline;
    });
    server.Register(fileno(stdin), [&server](int fd) {
        char sendline[MAXLINE];
        auto in = Fdopen(fd, "r");
        if (Fgets(sendline, MAXLINE, in) == nullptr)
            return "";
        Writen(server.getPeerFd(), sendline, strlen(sendline));
    });
    server.Run();
}
