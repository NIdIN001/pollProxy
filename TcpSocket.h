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

    void socketConnect(char *_hostName, int port);

    int socketRead(char *buf, int length) const;

    int socketWrite(char *buf, int length) const;

    void closeSocket() const;

    TcpSocket();

    char *getHostName();

private:
    void resolveHostName(char *hostName, int port, addrinfo **res);
};

