#pragma once
#include "Cache.h"
#include "TcpSocket.h"
#include "HttpProxy.h"

#include <iostream>
#include <fstream>
//#include <queue>
#include <string> 

class HttpProxy;
struct messageChunk;

class ClientSocketHandler {
private:
    TcpSocket *clientSocket;
    TcpSocket *hostSocket = nullptr;
    HttpProxy *proxy;

    std::list<messageChunk> messageQueue;

    bool recvChunk();

    bool sendChunk();

    bool parseRequest(char *request);

public:
    ClientSocketHandler(TcpSocket *clientSocket, HttpProxy *proxy);

    ~ClientSocketHandler();

    bool handle(PollResult pollResult);

    void setHostSocket(TcpSocket *hostSocket);

    int getClientFd();
};

