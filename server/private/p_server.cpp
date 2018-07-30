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

void getMessageStatistic(const char* msg, size_t length, int& sum, SetOfNumbers& numbers)
{
    numbers.clear();
    sum = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (msg[i] >= '0' && msg[i] <= '9')
        {
            sum += static_cast<int>(msg[i] - '0');
            numbers.emplace(static_cast<char>(msg[i] - '0'));
        }
    }
}

void printMessageStatistic(const char* info, int sum, const SetOfNumbers& numbers)
{
    std::cout << info << std::endl;
    if (numbers.empty())
    {
        std::cout << "Numbers not found" << "\n\n";
        return;
    }
    // ...
    std::cout << "sum: " << sum << std::endl;
    std::cout << "numbers: ";
    for (auto number : numbers)
        std::cout << (int)number << " ";
    std::cout << std::endl << "min: " << static_cast<int>(*numbers.rbegin());
    std::cout << std::endl << "max: " << static_cast<int>(*numbers.begin());
    std::cout << "\n\n";
}

} // namespace protei
