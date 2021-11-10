#include "HostSocketHandler.h"

HostSocketHandler::HostSocketHandler(TcpSocket * clientSocket, TcpSocket * hostSocket, Cache *cache, HttpProxy * proxy) {
    this->clientSocket = clientSocket;
    this->hostSocket = hostSocket;
    this->proxy = proxy;
    this->cache = cache;
    this->url = nullptr;
}

HostSocketHandler::~HostSocketHandler() {
}

bool HostSocketHandler::recvChunk() {
    char *buf = new char[MAX_CHUNK_SIZE];
    int length = hostSocket->_read(buf, MAX_CHUNK_SIZE);

    if (length == 0)
        return false;

    //add chunk to message queue
    messageChunk chunk;
    chunk.buf = buf;
    chunk.length = length;
    messageQueue.push_back(chunk);

    //add chunk to cache
    cache->addChunk(url, chunk);

    return true;
}

bool HostSocketHandler::sendChunk() {
    if (!messageQueue.empty()) {
        messageChunk chunk = messageQueue.front();
        //messageQueue.pop();
        messageQueue.pop_front();

        int length = clientSocket->_write(chunk.buf, chunk.length);
        if (length == 0)
            return false;
    }
    return true;
}

// returns false if session should be closed
bool HostSocketHandler::handle(PollResult pollResult) {
    int fd = pollResult.fd;
    int revents = pollResult.revents;

    if (revents & POLLHUP) {
        //peer closed its end of the channel
        return false;
    }

    if (fd == hostSocket->fd) {
        if (revents & POLLIN) {
            //we can read a chunk from host
            return recvChunk();

        }
    } else if (fd == clientSocket->fd) {
        if (revents & POLLOUT) {
            //we can send a chunk to client
            return sendChunk();
        }
    }

    return true;
}

void HostSocketHandler::finishReadingResponce() {
    if (url != nullptr) {
        //notify cache that entry with current url is full
        cache->makeEntryFull(url);

        std::cout << "HostSocketHandler with hostFd = " << hostSocket->fd << " has finished reading " << url
                  << std::endl;
    }
}

void HostSocketHandler::waitForNextResponce(char *url) {
    this->url = url;
    std::cout << "HostSocketHandler with hostFd = " << hostSocket->fd << " starting to read " << url << std::endl;
    cache->addEntry(url);
}
