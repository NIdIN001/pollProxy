#include "TcpSocket.h"

class ProxyServerSocket {
    static const int BacklogSize = 50;

    int fd;

public:
    explicit ProxyServerSocket(int port);

    ~ProxyServerSocket();

    int getFd() const;

    TcpSocket socketAccept() const;
};

