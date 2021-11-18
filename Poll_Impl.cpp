#include <libc.h>
#include "Poll_Impl.h"

Poll_Impl::Poll_Impl() = default;

Poll_Impl::~Poll_Impl() = default;

void Poll_Impl::addToPoll(int fd, short events) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
    pollfd pollfd{fd, events, 0};
    descriptors.push_back(pollfd);
}

void Poll_Impl::removeFromPoll(int fd) {
    for (int i = 0; i < descriptors.size(); i++) {
        if (descriptors[i].fd == fd) {
            descriptors.erase(descriptors.begin() + i);
            break;
        }
    }
}

void Poll_Impl::makePoll() {
    for (auto &desc : descriptors)
        desc.revents = 0;

    poll(descriptors.data(), descriptors.size(), -1);
}

std::vector<PollResult> Poll_Impl::getPollResult() {
    std::vector<PollResult> events;

    for (auto &desc : descriptors) {
        if (desc.revents != 0) {
            PollResult pollResult {desc.fd, desc.revents};
            events.push_back(pollResult);
        }
    }

    return events;
}
