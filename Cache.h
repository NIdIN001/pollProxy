#pragma once

#include "CacheRepository.h"
#include "entities/entities.h"

class CacheRepository;

class Cache {
    std::list<CacheRecord> records;
    std::vector<Listener> listeners;

public:
    void addEmptyRecord(char *url);

    void addPath(char *url, MessagePath chunk);

    bool containsRecord(char *url);

    std::list<MessagePath> getRecordByUrl(char *url);

    bool isRecordFull(char *url);

    void makeRecordFull(char *url);

    void listenToUrl(char *url, CacheRepository *listener);

    void stopListening(CacheRepository *listener);

    Cache();

    ~Cache();
};

