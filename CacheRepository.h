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

    void finishRead();

    bool isReading();

    void putToQueue(MessagePath msgPath);

    bool handlePollRevents(PollResult pollResult);

private:
    bool sendPath();
};
