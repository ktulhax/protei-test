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
#include <fcntl.h>

namespace protei
{

ServerUDP::ServerUDP(uint16_t port)
    : Server(socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP), port)
{
}

void serverUDPLoop(int serverSocket, std::atomic_bool& stopLoop)
{
    fd_set fds, fdsCopy;
    initFileDescriptors(&fds, &fdsCopy, serverSocket);

    sockaddr_in clientAddr;
    auto clientAddrLen = sizeof(clientAddr);
    std::vector<char> msg(SERVER_BUF_SIZE);
    fcntl(serverSocket, F_SETFD, O_NONBLOCK);
    while (!stopLoop)
    {
        fdsCopy = fds;
        if (!checkConnection(&fdsCopy, serverSocket))
        {
            std::cout << "waiting for udp connection" << std::endl;
            continue;
        }
        ssize_t rSize = recvfrom(serverSocket, msg.data(), msg.size(), 0, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        sendto(serverSocket, msg.data(), (rSize > 0 ? rSize : 0), 0, (sockaddr*)&clientAddr, clientAddrLen);
        std::cout << "Received packet from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
        std::cout << "Size of recieved message: " << rSize << std::endl;
    }
    stopLoop = false;
}

void ServerUDP::start()
{
    m_loop = std::thread(serverUDPLoop, m_socket, std::ref(m_stopLoop));
}

} // namespace protei
