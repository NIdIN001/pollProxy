#pragma once

#include "entities/entities.h"
#include <vector>
#include <sys/poll.h>

class Poll_Impl {
private:
    std::vector<pollfd> descriptors;

public:
    Poll_Impl();

    ~Poll_Impl();

    void addToPoll(int fd, short events);

    void removeFromPoll(int fd);

    void makePoll();

    std::vector<PollResult> getPollResult();
};
