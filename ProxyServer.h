#pragma once
#include <fcntl.h>

#include "Poll_Impl.h"
#include "entities/entities.h"
#include "ProxyServerSocket.h"
#include "ClientSocketHandler.h"
#include "DestServerSocketHandler.h"
#include "CacheRepository.h"

class ClientSocketHandler;
class DestServerSocketHandler;
class Cache;
class CacheRepository;

class ProxyServer {
    static const int DefaultDestPort = 80;

    Poll_Impl poller;
    ProxyServerSocket serverSocket;
    Cache *cache;
    std::vector<ProxyTunnel> proxyTunnels;

public:
    explicit ProxyServer(int port);

    ~ProxyServer();

    [[noreturn]] void run();

    void newRequest(ClientSocketHandler *clientSocketHandler, char *url);

    void makeHttpsTunnel(ClientSocketHandler *clientSocketHandler, char *url);

private:
    void closeConnection(int numberOfConnection);

    void acceptClient();
};

