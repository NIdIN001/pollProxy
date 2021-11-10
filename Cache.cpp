#include "Cache.h"

void Cache::addEntry(char * url) {
    cacheEntry cacheEntry;
    cacheEntry.url = url;
    cacheEntry.isFull = false;

    entries.push_front(cacheEntry);
}

void Cache::addChunk(char * url, messageChunk chunk) {
    //push chunk
    for (cacheEntry &it: entries) {
        if (strcmp(it.url, url) == 0) {
            it.chunks.push_back(chunk);
            break;
        }
    }

    //notify listeners
    for (listenerEntry listenerEntry : listeners) {
        if (strcmp(listenerEntry.url, url) == 0) {
            listenerEntry.listener->notify(chunk);
        }
    }
}

bool Cache::contains(char * url) {
    for (cacheEntry &it: entries) {
        if (strcmp(it.url, url) == 0) {
            return true;
        }
    }

    return false;
}

std::list<messageChunk> Cache::getChunks(char * url) {
    for (cacheEntry &it: entries) {
        if (strcmp(it.url, url) == 0)
            return it.chunks;
    }

    return std::list<messageChunk>();
}

bool Cache::entryIsFull(char * url) {
    for (cacheEntry &it: entries) {
        if (strcmp(it.url, url) == 0)
            return it.isFull;
    }

    return false;
}

void Cache::makeEntryFull(char * url) {
    for (cacheEntry &ref: entries) {
        if (strcmp(ref.url, url) == 0) {
            ref.isFull = true;
            return;
        }
    }
}

void Cache::listenToUrl(char * url, CacheReader * listener) {
    listenerEntry listenerEntry;
    listenerEntry.url = url;
    listenerEntry.listener = listener;

    listeners.push_back(listenerEntry);
}

void Cache::stopListening(CacheReader * listener) {
    for (int i = 0; i < listeners.size(); i++) {
        if (listeners[i].listener == listener)
            listeners.erase(listeners.begin() + i);
    }
}

void Cache::clear() {
    for (cacheEntry cacheEntry : entries) {
        delete[] cacheEntry.url;

        while (!cacheEntry.chunks.empty()) {
            delete[] cacheEntry.chunks.front().buf;
            delete[] cacheEntry.url;
            cacheEntry.chunks.erase(cacheEntry.chunks.begin());
        }
    }
}

Cache::Cache()
{
}


Cache::~Cache()
{
}