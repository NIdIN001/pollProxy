#include "ClientSocketHandler.h"

ClientSocketHandler::ClientSocketHandler(TcpSocket *clientSocket, ProxyServer *proxy) :
        clientSocket(clientSocket), proxyServer(proxy) {}

ClientSocketHandler::~ClientSocketHandler() = default;

bool ClientSocketHandler::parseRequest(char *req) {
    if (strstr(req, "CONNECT") == req)
        return false;

    if (strstr(req, "GET") == req) {
        const char *tmpUrl = strstr(req, "http://") + 7;

        char url[URL_MAX_LEN];
        int i = 0;
        for (i = 0; tmpUrl[i] != ' '; i++) {
            url[i] = tmpUrl[i];
        }
        url[i] = '\0';

        proxyServer->newRequest(this, url);
        return true;
    }

    return true;
}


bool ClientSocketHandler::recvPath() {
    char *buf = new char[BUFFER_SIZE];
    int len = clientSocket->socketRead(buf, BUFFER_SIZE);

    std::cout << "RECV:" << buf << std::endl;

    if (len == 0)
        return false;

    MessagePath msgPath{buf, len};
    messageQueue.push_back(msgPath);

    return parseRequest(buf);
}

bool ClientSocketHandler::sendPath() {
    if (!messageQueue.empty()) {
        std::cout << "ClientSocketHandler messageQueue is not empty" << std::endl;
        MessagePath msgPath = messageQueue.front();
        messageQueue.pop_front();

        int len = destServerSocket->socketWrite(msgPath.data, msgPath.length);
        std::cout << "send " << len << " bytes" << std::endl;
        if (len == 0)
            return false;
    }

    return true;
}

bool ClientSocketHandler::handlePollResult(PollResult pollResult) {
    if (pollResult.revents & POLLHUP)
        return false;

    if (pollResult.fd == clientSocket->getFd()) {
        if (pollResult.revents & POLLIN)
            return recvPath();
    } else if (destServerSocket != nullptr && pollResult.fd == destServerSocket->getFd()) {
        if (pollResult.revents & POLLOUT)
            return sendPath();
    }

    return true;
}

void ClientSocketHandler::setDestServerSocket(TcpSocket *destServer) {
    this->destServerSocket = destServer;
}
