#include "server.h"
#include <iostream>
#include <unistd.h>

void usage()
{
    std::cout << "./server tcp_port udp_port" << std::endl;
}

void waitForExit()
{
    std::cout << "Type 'exit' to terminate application...\n\n";
    std::string cmd;
    while(cmd != "exit")
        std::cin >> cmd;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        usage();
        return 0;
    }
    try
    {
        uint16_t tcpPort = std::atoi(argv[1]);
        uint16_t udpPort = std::atoi(argv[2]);
        if (tcpPort == udpPort)
        {
            std::cout << "Error: ports are equal";
            return 1;
        }
        auto tcpServer = std::shared_ptr<protei::Server>(protei::Server::createServer("tcp", tcpPort));
        tcpServer->start();
        auto udpServer = std::shared_ptr<protei::Server>(protei::Server::createServer("udp", udpPort));
        udpServer->start();
        waitForExit();
    }
    catch(std::runtime_error& err)
    {
        std::cout << err.what()<< std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
