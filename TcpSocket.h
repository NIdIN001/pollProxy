#pragma once

#include <unistd.h>
#include <netdb.h>

class TcpSocket {
    int fd;
    char *hostName;

public:
    explicit TcpSocket(int fd);

    ~TcpSocket();

    int getFd() const;

    void socketConnect(char *_hostUrl, int port);

    int socketRead(char *buffer, int len) const;

    int socketWrite(char *buffer, int len) const;

    void closeSocket() const;

    TcpSocket();

    char *getHostName();

private:
    void resolveHostName(char *hostUrl, int port, addrinfo **result);
};

