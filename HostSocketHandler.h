#pragma once
#include "TcpSocket.h"
#include "Cache.h"
#include "HttpProxy.h"

class HttpProxy;
class Cache;
struct messageChunk;

class HostSocketHandler {
private:
    TcpSocket *clientSocket;
    TcpSocket *hostSocket;
    HttpProxy *proxy;
    std::list<messageChunk> messageQueue;
    Cache *cache;
    char *url;

    bool recvChunk();

    bool sendChunk();

public:
    HostSocketHandler(TcpSocket *clientSocket, TcpSocket *hostSocket, Cache *cache, HttpProxy *proxy);

    ~HostSocketHandler();

    bool handle(PollResult pollResult);

    void finishReadingResponce();

    void waitForNextResponce(char *url);
};

