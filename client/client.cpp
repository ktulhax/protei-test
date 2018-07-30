#include "client.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

namespace protei
{

Client::Client(Protocol prt, const char* sServerAddress, uint16_t serverPort)
    : m_socket(-1)
{
    if (prt == Protocol::TCP)
        m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    else
        m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_socket == -1)
        throw std::runtime_error("socket creating error");

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof (serverAddress));
    serverAddress.sin_family = PF_INET;
    serverAddress.sin_port = htons(serverPort);

    if (inet_pton(PF_INET, sServerAddress, &serverAddress.sin_addr) <= 0)
        throw std::runtime_error("ip address initialization error");

    if (::connect(m_socket, (struct sockaddr*) &serverAddress, sizeof (serverAddress)) == -1)
        throw std::runtime_error("connection error");

    fcntl(m_socket, F_SETFL, O_NONBLOCK);
}

Client::~Client()
{
    close(m_socket);
}

bool Client::readAllBytes(std::string& msg)
{
    ssize_t rSize = 0;
    std::vector<char> buffer(CLIENT_BUF_SIZE);
    while ((rSize = recv(m_socket, buffer.data(), buffer.size(), 0)) > 0)
        msg.insert(msg.end(), buffer.begin(), buffer.begin() + rSize);
    return (rSize != 0);
}

bool Client::send(const char* msg, size_t length)
{
    auto rSize = ::send(m_socket, msg, length, 0);
    std::string echoMsg;
    bool flag = readAllBytes(echoMsg);
    std::cout << "Size of recieved message: " << echoMsg.size() << std::endl;
    return flag;
}

} // namespace protei
