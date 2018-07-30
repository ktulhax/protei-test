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
    while (!stopLoop)
    {
        fdsCopy = fds;
        if (!checkConnection(&fdsCopy, serverSocket))
            continue;
        ssize_t rSize = recvfrom(serverSocket, msg.data(), msg.size(), 0, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        sendto(serverSocket, msg.data(), (rSize > 0 ? rSize : 0), 0, (sockaddr*)&clientAddr, clientAddrLen);
        int sum;
        SetOfNumbers numbers;
        getMessageStatistic(msg.data(), rSize, sum, numbers);
        printMessageStatistic("Recieved udp message...", sum, numbers);
    }
    stopLoop = false;
}

void ServerUDP::start()
{
    m_loop = std::thread(serverUDPLoop, m_socket, std::ref(m_stopLoop));
}

} // namespace protei
