#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/in.h>

namespace protei
{

class Client final
{
public:
    enum class Protocol { TCP, UDP };
public:
    Client(Protocol prt, const char* serverAddress, uint16_t serverPort);
    void connect();
    void send(const char* msg, size_t length);
    ~Client();
private:
    int m_socket;
    sockaddr_in m_serverAddress;
};

} // namespace protei

#endif // CLIENT_H
