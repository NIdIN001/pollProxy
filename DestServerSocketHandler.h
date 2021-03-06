#pragma once
#include "ProxyServer.h"

class ProxyServer;
class Cache;
class MessagePath;

class DestServerSocketHandler {
    static const int BUFFER_SIZE = 65536;

    TcpSocket *clientSocket;
    TcpSocket *destServerSocket;
    std::list<MessagePath> messageQueue;
    Cache *cache;
    char *url;

    bool recvPath();

    bool sendPath();

public:
    DestServerSocketHandler(TcpSocket *clientSocket, TcpSocket *destServer, Cache *cache);

    ~DestServerSocketHandler();

    bool handlePollRevents(PollResult pollResult);

    void endReadingResponce();

    void waitForNextResponce(char *url);
};
