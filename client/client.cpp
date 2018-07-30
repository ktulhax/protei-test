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

namespace protei
{

Client::Client(Protocol prt, const char* serverAddress, uint16_t serverPort)
{
    if (prt == Protocol::TCP)
        m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    else
        m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_socket == -1)
        throw std::runtime_error("socket creating error");

    memset(&m_serverAddress, 0, sizeof (m_serverAddress));
    m_serverAddress.sin_family = PF_INET;
    m_serverAddress.sin_port = htons(serverPort);
    auto err = inet_pton(PF_INET, serverAddress, &m_serverAddress.sin_addr);

    if (err <= 0)
        throw std::runtime_error("ip address initialization error");
}

Client::~Client()
{
    close(m_socket);
}

void Client::connect()
{
    if (::connect(m_socket, (struct sockaddr*) &m_serverAddress, sizeof (m_serverAddress)) == -1)
        throw std::runtime_error("connection error");
}

void Client::send(const char* msg, size_t length)
{
    auto rSize = ::send(m_socket, msg, length, 0);
    std::vector<char> echoMsg(0x80000);
    rSize = recv(m_socket, echoMsg.data(), echoMsg.size(), 0);
    std::cout << "Size of recieved message: " << rSize << std::endl;
}

} // namespace protei
