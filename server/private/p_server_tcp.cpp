#include "server.h"
#include <fstream>
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

enum class TCPState {
    Success,
    Fail,
    EndOfSession
};

TCPState readAllBytes(int sock, std::vector<char>& msg)
{
    bool readFlag = false;
    std::vector<char> buffer(SERVER_BUF_SIZE);
    ssize_t rSize = 0;
    while ((rSize = recv(sock, buffer.data(), buffer.size(), 0)) > 0)
    {
        msg.insert(msg.end(), buffer.begin(), buffer.begin() + rSize);
        readFlag = true;
    }
    if (readFlag)
        return TCPState::Success;
    else
        return (rSize == 0 ? TCPState::EndOfSession : TCPState::Fail);
}

void serverTCPConnection(int clientSocket, std::atomic_bool& stopLoop)
{
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    while (!stopLoop)
    {
        std::vector<char> msg;
        auto res = readAllBytes(clientSocket, msg);
        if (res == TCPState::Success)
        {
            std::cout << "Size of recieved message: " << msg.size() << std::endl;
            std::cout << "Recieved message: " << msg.data() << std::endl;
            send(clientSocket, msg.data(), msg.size(), 0);
            int sum = 0;
            SetOfNumbers numbers;
            getMessageStatistic(msg.data(), msg.size(), sum, numbers);
            printMessageStatistic("Recieved tcp message", sum, numbers);
        }
        else if (res == TCPState::EndOfSession)
            break;
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void serverTCPLoop(int serverSocket, std::atomic_bool& stopLoop)
{
    std::vector<std::future<void>> handles;
    fd_set fds, fdsCopy;
    initFileDescriptors(&fds, &fdsCopy, serverSocket);

    auto checkHandlesState = [&handles]() {
        for (auto it = handles.begin(); it != handles.end(); ) {
            if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                it = handles.erase(it);
            else
                it++;
        }
    };
    auto waitEndOfConnections = [&handles]() {
        for (auto& h : handles)
            h.wait();
    };
    while (!stopLoop)
    {
        fdsCopy = fds;
        checkHandlesState();
        if (!checkConnection(&fdsCopy, serverSocket))
            continue;
        int clientSocket = accept(serverSocket, 0, 0);
        if (clientSocket < 0)
            throw std::runtime_error("tcp accepting error");
        // start thread for new tcp connection
        handles.push_back(std::async(std::launch::async, serverTCPConnection, clientSocket, std::ref(stopLoop)));
    }
    waitEndOfConnections();
    stopLoop = false;
}

void ServerTCP::start()
{
    m_loop = std::thread(serverTCPLoop, m_socket, std::ref(m_stopLoop));
}

ServerTCP::ServerTCP(uint16_t port)
    : Server(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP), port)
{
    if (listen(m_socket, 10) == -1)
        throw std::runtime_error("listening error");
}

} // namespace protei
