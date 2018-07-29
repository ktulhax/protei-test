#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void runUDP(uint16_t port)
{
    sockaddr_in serverAddr;
    int clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (clientSocket == -1)
        throw std::runtime_error("socket creating error");

    memset(&serverAddr, 0, sizeof (serverAddr));

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    auto err = inet_pton(PF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (err <= 0)
    {
        close(clientSocket);
        throw std::runtime_error("ip address intialization error");
    }

    if (connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof (serverAddr)) == -1) {
        close(clientSocket);
        throw std::runtime_error("connection error");
    }

    send(clientSocket, "hello server", strlen("hello server") + 1, 0);

    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void run(uint16_t port)
{
    sockaddr_in serverAddr;
    int clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == -1)
        throw std::runtime_error("socket creating error");

    memset(&serverAddr, 0, sizeof (serverAddr));

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(port);
    auto err = inet_pton(PF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (err <= 0)
    {
        close(clientSocket);
        throw std::runtime_error("ip address intialization error");
    }

    if (connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof (serverAddr)) == -1) {
        close(clientSocket);
        throw std::runtime_error("connection error");
    }

    send(clientSocket, "hello server", strlen("hello server") + 1, 0);

    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

int main(void) {
    try
    {
        run(12312);
    }
    catch(std::runtime_error& err)
    {
        std::cout << err.what()<< std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "ok"<< std::endl;
    return 0;
}
