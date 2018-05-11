#include "unp.h"
#include "server.hpp"

int main()
{
    auto server = Server::New();
    server.Listen(8888);
    server.Register(Server::CONN_FD,[](int fd)->{})
    server.Run();
}