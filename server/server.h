#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <thread>
#include <atomic>

namespace protei
{

class Server
{
public:
    static Server* createServer(const char* protocol, uint16_t port);
    virtual void start() = 0;
    void stop();
    virtual ~Server();
protected:
    Server(int socket, uint16_t port);
protected:
    int m_socket;
    std::thread m_loop;
    std::atomic_bool m_stopLoop;
};

} // namespace protei

#endif // SERVER_H
