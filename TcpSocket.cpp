#include "TcpSocket.h"
#include <iostream>

TcpSocket::TcpSocket() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpSocket::TcpSocket(int fd) {
    this->fd = fd;
}

TcpSocket::~TcpSocket() = default;

char* TcpSocket::getHostName() {
    return hostName;
}

void TcpSocket::resolveHostName(char* hostUrl, int port, addrinfo **result) {
    addrinfo addr;
    memset(&addr, 0, sizeof(addr));
    addr.ai_socktype = SOCK_STREAM;
    addr.ai_family = AF_INET;
    addr.ai_flags = AI_CANONNAME;

    if (getaddrinfo(hostUrl, std::to_string(port).c_str(), &addr, result) != 0)
        std::cout << "Can't resolve " << hostUrl << std::endl;
}

void TcpSocket::socketConnect(char* _hostUrl, int port) {
    hostName = _hostUrl;
    addrinfo *hostAddrInfo;

    resolveHostName(hostName, port, &hostAddrInfo);

    if (connect(fd, hostAddrInfo->ai_addr, hostAddrInfo->ai_addrlen) != 0)
        std::cout << "Can't connect to  " << hostName << ":" << port << std::endl;
}

void TcpSocket::closeSocket() const {
    close(fd);
}

int TcpSocket::getFd() const {
    return fd;
}

int TcpSocket::socketRead(char *buffer, int len) const {
    return read(fd, buffer, len);
}

int TcpSocket::socketWrite(char *buffer, int len) const {
    return write(fd, buffer, len);
}
