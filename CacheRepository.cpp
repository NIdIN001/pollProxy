#include "CacheRepository.h"

CacheRepository::~CacheRepository() = default;

bool CacheRepository::sendPath() {
    if (!messageQueue.empty()) {
        messageQueue.pop_front();

        MessagePath msgPath = messageQueue.front();
        int length = writeSocket->socketWrite(msgPath.data, msgPath.length);
        if (length == 0)
            return false;
    }
    return true;
}


CacheRepository::CacheRepository(Cache *cache, TcpSocket *writeSocket) {
    this->writeSocket = writeSocket;
    this->cache = cache;
    this->url = nullptr;
}

void CacheRepository::read(char *_url) {
    this->url = _url;
    messageQueue = cache->getRecordByUrl(url);

    if (cache->isRecordFull(url)) {
        std::cout << "Data from ready cache: " << url << std::endl;
    } else {
        std::cout << "Data from doesn't ready cache: " << url << std::endl;
        cache->listenToUrl(_url, this);
    }
}

void CacheRepository::finishRead() {
    if (url != nullptr) {
        cache->stopListening(this);
        std::cout << "Cache of " << url << " is ready" << std::endl;
        url = nullptr;
    }
}

bool CacheRepository::isReading() {
    return url != nullptr;
}

void CacheRepository::putToQueue(MessagePath msgPath) {
    messageQueue.push_back(msgPath);
}

bool CacheRepository::handlePollRevents(PollResult pollResult) {
    if (pollResult.fd != writeSocket->getFd())
        return true;

    if (pollResult.revents & POLLHUP) {
        return false;
    } else if (pollResult.revents & POLLOUT) {
        return sendPath();
    }

    return true;
}
