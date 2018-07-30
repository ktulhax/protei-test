#ifndef CLIENT_H
#define CLIENT_H

namespace protei
{

class Client
{
public:
    static Client* createClient(const char* protocol);
protected:
    Client() {}
    virtual ~Client();
protected:
    int m_socket;
};

} // namespace protei

#endif // CLIENT_H
