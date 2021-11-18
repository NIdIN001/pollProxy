#include "Cache.h"

Cache::Cache() = default;

Cache::~Cache() = default;

void Cache::addEmptyRecord(char *url) {
    CacheRecord cacheRecord;
    cacheRecord.isFull = false;
    cacheRecord.url = url;

    records.push_front(cacheRecord);
}

void Cache::addPath(char *url, MessagePath msgPath) {
    for (CacheRecord &record: records) {
        if (strcmp(record.url, url) == 0) {
            record.paths.push_back(msgPath);
            break;
        }
    }

    for (Listener listener : listeners) {
        if (strcmp(listener.url, url) == 0) {
            listener.listener->putToQueue(msgPath);
        }
    }
}

bool Cache::containsRecord(char *url) {
    for (CacheRecord &record: records) {
        if (strcmp(record.url, url) == 0) {
            return true;
        }
    }

    return false;
}

std::list<MessagePath> Cache::getRecordByUrl(char *url) {
    for (CacheRecord &record: records) {
        if (strcmp(record.url, url) == 0)
            return record.paths;
    }

    return std::list<MessagePath>();
}

bool Cache::isRecordFull(char *url) {
    for (CacheRecord &record: records) {
        if (strcmp(record.url, url) == 0)
            return record.isFull;
    }

    return false;
}

void Cache::makeRecordFull(char *url) {
    for (CacheRecord &record: records) {
        if (strcmp(record.url, url) == 0) {
            record.isFull = true;
            return;
        }
    }
}

void Cache::listenToUrl(char *url, CacheRepository *_listener) {
    Listener listener{_listener, url};

    listeners.push_back(listener);
}

void Cache::stopListening(CacheRepository *listener) {
    for (int i = 0; i < listeners.size(); i++) {
        if (listeners[i].listener == listener)
            listeners.erase(listeners.begin() + i);
    }
}
