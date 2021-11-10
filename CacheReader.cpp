#include "CacheReader.h"

bool CacheReader::sendChunk() {
    if (!messageQueue.empty()) {
        messageChunk chunk = messageQueue.front();
        messageQueue.pop_front();

        int length = writeSocket->_write(chunk.buf, chunk.length);
        if (length == 0)
            return false;
    }
    return true;
}


CacheReader::CacheReader(Cache *cache, TcpSocket *writeSocket, HttpProxy *proxy) {
    this->cache = cache;
    this->writeSocket = writeSocket;
    this->proxy = proxy;
    this->url = nullptr;
}


CacheReader::~CacheReader()
{
}

void CacheReader::read(char* url) {
    this->url = url;
    messageQueue = cache->getChunks(url);

    if (cache->entryIsFull(url)) {
        std::cout << "Cache reader with write socket fd = " << writeSocket->fd << " is reading " << url
                  << " (entry for this URL is full)" << std::endl;
    } else {
        std::cout << "Cache reader with write socket fd = " << writeSocket->fd << " is reading " << url
                  << " (entry for this URL isn't full)" << std::endl;
        cache->listenToUrl(url, this);
    }
}

void CacheReader::stopRead() {
    if (url != nullptr) {
        cache->stopListening(this);
        std::cout << "Cache reader with write socket fd = " << writeSocket->fd << " has finished reading " << url
                  << std::endl;
        url = nullptr;
    }
}

bool CacheReader::isReading() {
    return url != nullptr;
}

void CacheReader::notify(messageChunk chunk){
	//messageQueue.push(chunk);
	messageQueue.push_back(chunk);
}

bool CacheReader::handle(PollResult pollResult) {
    if (pollResult.fd != writeSocket->fd)
        return true;

    if (pollResult.revents & POLLHUP) {
        return false;
    } else if (pollResult.revents & POLLOUT) {
        return sendChunk();
    }

    return true;
}
