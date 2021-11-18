#pragma once
#include "ProxyServer.h"

class ProxyServer;
struct MessagePath;

class ClientSocketHandler {
    static const int BUFFER_SIZE = 65536;
    static const int URL_MAX_LEN = 4096;

    TcpSocket *clientSocket = nullptr;
    TcpSocket *destServerSocket = nullptr;
    ProxyServer *proxyServer;

    std::list<MessagePath> messageQueue;

    bool recvChunk();

    bool sendChunk();

    bool parseRequest(char *req);

public:
    ClientSocketHandler(TcpSocket *clientSocket, ProxyServer *proxy);

    ~ClientSocketHandler();

    bool handlePollResult(PollResult pollResult);

    void setDestServerSocket(TcpSocket *destServer);

    int getClientFd();
};

