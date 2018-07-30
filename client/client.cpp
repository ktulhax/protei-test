#include "client.h"
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

class ClientTCP : public Client
{
public:
    ClientTCP();
};

class ClientUDP : public Client
{
public:
    ClientUDP();
};

ClientTCP::ClientTCP()
{
}

ClientUDP::ClientUDP()
{

}

Client::~Client()
{
    close(m_socket);
}

Client* Client::createClient(const char* protocol)
{
    if (::strcmp("tcp", protocol) == 0)
        return new ClientTCP();
    else if (::strcmp("udp", protocol) == 0)
        return new ClientUDP();
    else
        return 0;
}

} // namespace protei
