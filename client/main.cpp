#include "client.h"
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

void usage()
{
    std::cout << "./client [tcp|udp] server_ipv4 server_port" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        usage();
        return 0;
    }
    try
    {
        protei::Client::Protocol proto;
        if (::strcmp(argv[1], "tcp") == 0)
            proto = protei::Client::Protocol::TCP;
        else if (::strcmp(argv[1], "udp") == 0)
            proto = protei::Client::Protocol::UDP;
        else
        {
            std::cout << "Incorrect protocol" << std::endl;
            return 1;
        }
        const char* ipaddr = argv[2];
        const uint16_t port = std::stoi(argv[3]);
        protei::Client c(proto, ipaddr, port);
        c.connect();
        c.send("aloha1", strlen("aloha1") + 1);
        // c.connect();
        c.send("aloha12", strlen("aloha12") + 1);
    }
    catch(std::runtime_error& err)
    {
        std::cout << err.what()<< std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}
