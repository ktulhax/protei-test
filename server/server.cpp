#include "server.h"
#include "p_server.h"
#include <future>
#include <functional>
#include <vector>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>

namespace protei
{

Server* Server::createServer(const char* protocol, uint16_t port)
{
    if (::strcmp("tcp", protocol) == 0)
        return new ServerTCP(port);
    else if (::strcmp("udp", protocol) == 0)
        return new ServerUDP(port);
    else
        return 0;
}

Server::Server(int socket, uint16_t port)
    : m_socket(socket)
    , m_stopLoop(false)
{
    if (m_socket == -1)
        throw std::runtime_error("creating socket error");

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int flag = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (bind(m_socket, (sockaddr*)&serverAddr, sizeof (serverAddr)) == -1)
        throw std::runtime_error("binding error");
}

Server::~Server()
{
    stop();
    if (m_socket != -1)
        close(m_socket);
}

void Server::stop()
{
    m_stopLoop = true;
    while(m_stopLoop);
    m_loop.join();
}

} // namespace protei
