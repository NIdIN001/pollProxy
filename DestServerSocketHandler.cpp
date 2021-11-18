#include "DestServerSocketHandler.h"

DestServerSocketHandler::DestServerSocketHandler(TcpSocket *clientSocket, TcpSocket *hostSocket, Cache *cache) {
    this->url = nullptr;
    this->clientSocket = clientSocket;
    this->destServerSocket = hostSocket;
    this->cache = cache;
}

DestServerSocketHandler::~DestServerSocketHandler() = default;

bool DestServerSocketHandler::recvPath() {
    char *buffer = new char[BUFFER_SIZE];
    int len = destServerSocket->socketRead(buffer, BUFFER_SIZE);

    if (len == 0)
        return false;

    MessagePath msgPath{buffer, len};

    messageQueue.push_back(msgPath);
    cache->addPath(url, msgPath);

    return true;
}

bool DestServerSocketHandler::sendPath() {
    if (!messageQueue.empty()) {
        std::cout << "DestServerSocketHandler messageQueue is not empty" << std::endl;
        MessagePath msgPath = messageQueue.front();
        messageQueue.pop_front();

        int len = clientSocket->socketWrite(msgPath.data, msgPath.length);
        if (len == 0)
            return false;

        std::cout << "send " << len << " bytes" << std::endl;
    }

    return true;
}

bool DestServerSocketHandler::handle(PollResult pollResult) {
    if (pollResult.revents & POLLHUP)
        return false;

    if (pollResult.fd == destServerSocket->getFd()) {
        if (pollResult.revents & POLLIN)
            return recvPath();

    } else if (pollResult.fd == clientSocket->getFd()) {
        if (pollResult.revents & POLLOUT)
            return sendPath();
    }

    return true;
}

void DestServerSocketHandler::endReadingResponce() {
    if (url != nullptr)
        cache->makeRecordFull(url);
}

void DestServerSocketHandler::waitForNextResponce(char *url) {
    this->url = url;
    cache->addEmptyRecord(url);
}
