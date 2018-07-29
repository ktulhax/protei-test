#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

void runUDP(uint16_t port)
{
    sockaddr_in serverAddr, clientAddr;
    auto clientAddrLen = sizeof(clientAddr);
    auto serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serverSocket == -1)
        throw std::runtime_error("creating socket error");

    memset(&serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof (serverAddr)) == -1)
    {
        close(serverSocket);
        throw std::runtime_error("binding error");
    }

    while (true)
    {
        std::vector<char> buf(524288);
        auto rSize = recvfrom(serverSocket, buf.data(), buf.size(), 0, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        std::cout << "Received packet from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
        std::cout << "Recieved message: " << buf.data() << std::endl;
    }
    close(serverSocket);
}


void run(uint16_t port)
{
    sockaddr_in serverAddr;
    auto serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == -1)
        throw std::runtime_error("creating socket error");

    memset(&serverAddr, 0, sizeof (serverAddr));
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof (serverAddr)) == -1)
    {
        close(serverSocket);
        throw std::runtime_error("binding error");
    }

    if (listen(serverSocket, 10) == -1)
    {
        close(serverSocket);
        throw std::runtime_error("listening error");
    }

    while (true)
    {
        int clientSocket = accept(serverSocket, 0, 0);
        if (clientSocket < 0)
        {
            close(serverSocket);
            throw std::runtime_error("accepting error");
        }
        std::vector<char> buf(524288);
        auto rSize = recv(clientSocket, buf.data(), buf.size(), 0);
        std::cout << "recieved message: " << buf.data() << std::endl;
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
    }
}

int main(int argc, char** argv)
{
    try
    {
        run(12312);
    }
    catch(std::runtime_error& err)
    {
        std::cout << err.what()<< std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "ok" << std::endl;
    return 0;
}
