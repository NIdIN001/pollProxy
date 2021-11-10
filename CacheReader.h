#pragma once
#include <vector>
#include <iostream>
#include <list>

#include "TcpSocket.h"
#include "Cache.h"
#include "HttpProxy.h"

class HttpProxy;
class Cache;
struct messageChunk;

class CacheReader {
private:
    Cache *cache;
    TcpSocket *writeSocket;
    HttpProxy *proxy;
    char *url;
    std::list<messageChunk> messageQueue;

    bool sendChunk();

public:
    CacheReader(Cache *cache, TcpSocket *writeSocket, HttpProxy *proxy);

    ~CacheReader();

    void read(char *url);

    void stopRead();

    bool isReading();

    void notify(messageChunk chunk);

    bool handle(PollResult pollresult);
};

