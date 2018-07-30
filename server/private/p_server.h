#ifndef P_SERVER_H
#define P_SERVER_H

#include "server.h"
#define SERVER_BUF_SIZE 0x80000

namespace protei
{

class ServerTCP final : public Server
{
public:
    ServerTCP(uint16_t port);
    void start() override;
private:
};

class ServerUDP final : public Server
{
public:
    ServerUDP(uint16_t port);
    void start() override;
private:
};

void initFileDescriptors(fd_set* pFDs, fd_set* pFDsCopy, int socket);
bool checkConnection(fd_set* pFDs, int serverSocket);

}

#endif // P_SERVER_H
