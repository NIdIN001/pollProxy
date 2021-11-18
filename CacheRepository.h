#pragma once

#include <vector>
#include <iostream>

#include "Cache.h"
#include "ProxyServer.h"

class ProxyServer;

class Cache;

class MessagePath;

class CacheRepository {
    Cache *cache;
    TcpSocket *writeSocket;
    char *url;

    std::list<MessagePath> messageQueue;

public:
    CacheRepository(Cache *cache, TcpSocket *writeSocket);

    ~CacheRepository();

    void read(char *_url);

    void stopRead();

    bool isReading();

    void notify(MessagePath chunk);

    bool handle(PollResult pollresult);

private:
    bool sendChunk();
};
