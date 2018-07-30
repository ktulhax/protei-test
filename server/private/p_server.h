#ifndef P_SERVER_H
#define P_SERVER_H

#include "server.h"
#include <set>
#define SERVER_BUF_SIZE 0x10000

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

using SetOfNumbers = std::multiset<char, std::greater<char>>;

void initFileDescriptors(fd_set* pFDs, fd_set* pFDsCopy, int socket);
bool checkConnection(fd_set* pFDs, int serverSocket);
void getMessageStatistic(const char* msg, size_t length, int& sum, SetOfNumbers& numbers);
void printMessageStatistic(const char* info, int sum, const SetOfNumbers& numbers);

}

#endif // P_SERVER_H
