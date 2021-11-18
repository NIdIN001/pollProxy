#include "ProxyServerSocket.h"
#include <iostream>

ProxyServerSocket::ProxyServerSocket(int port) {
    int tmp = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = AF_INET;


    if (bind(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0)
        std::cout << "Can't bind" << std::endl;

    listen(fd, BacklogSize);
}

ProxyServerSocket::~ProxyServerSocket() = default;

TcpSocket ProxyServerSocket::socketAccept() const {
    int socket = accept(fd, (struct sockaddr *) nullptr, nullptr);
    TcpSocket tcpSocket(socket);
    return tcpSocket;
}

int ProxyServerSocket::getFd() const {
    return fd;
}
