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

namespace protei
{

void initFileDescriptors(fd_set* pFDs, fd_set* pFDsCopy, int serverSocket)
{
    FD_ZERO(pFDs);
    FD_ZERO(pFDsCopy);
    FD_SET(serverSocket, pFDs);
}

bool checkConnection(fd_set* pFDs, int serverSocket)
{
    timeval tv = { 3, 500000 };
    if (select(serverSocket + 1, pFDs, NULL, NULL, &tv) == -1)
        throw std::runtime_error("");
    return FD_ISSET(serverSocket, pFDs);
}

} // namespace protei
