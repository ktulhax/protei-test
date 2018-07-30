#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdint.h>
#include <netinet/in.h>

#define CLIENT_BUF_SIZE 0x10000

namespace protei
{

class Client final
{
public:
    enum class Protocol { TCP, UDP };
public:
    Client(Protocol prt, const char* serverAddress, uint16_t serverPort);
    bool send(const char* msg, size_t length);
    ~Client();
private:
    bool readAllBytes(std::string& msg);
private:
    int m_socket;
};

} // namespace protei

#endif // CLIENT_H
