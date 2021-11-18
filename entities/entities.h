#pragma once

#include <list>

class CacheRepository;
class ClientSocketHandler;
class DestServerSocketHandler;
class TcpSocket;

class PollResult {
public:
    int fd;
    int revents;
};

class ProxyTunnel {
public:
    CacheRepository *cacheReader;

    TcpSocket *clientSocket;
    ClientSocketHandler *clientSocketHandler;

    TcpSocket *remoteServerSocket;
    DestServerSocketHandler *remoteServerSocketHandler;
};

class MessagePath {
public:
    char *data;
    int length;
};

class CacheRecord {
public:
    std::list<MessagePath> paths;
    char *url;
    bool isFull;
};

class Listener {
public:
    CacheRepository *listener;
    char *url;
};
