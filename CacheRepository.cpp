#include "CacheRepository.h"

CacheRepository::~CacheRepository() = default;

bool CacheRepository::sendChunk() {
    if (!messageQueue.empty()) {
        messageQueue.pop_front();

        MessagePath chunk = messageQueue.front();
        int length = writeSocket->socketWrite(chunk.data, chunk.length);
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

void CacheRepository::stopRead() {
    if (url != nullptr) {
        cache->stopListening(this);
        std::cout << "Cache of " << url << " is ready" << std::endl;
        url = nullptr;
    }
}

bool CacheRepository::isReading() {
    return url != nullptr;
}

void CacheRepository::notify(MessagePath chunk) {
    messageQueue.push_back(chunk);
}

bool CacheRepository::handle(PollResult pollResult) {
    if (pollResult.fd != writeSocket->getFd())
        return true;

    if (pollResult.revents & POLLHUP) {
        return false;
    } else if (pollResult.revents & POLLOUT) {
        return sendChunk();
    }

    return true;
}
