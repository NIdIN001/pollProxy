#include "Poller.h"

Poller::Poller() {
}

Poller::~Poller() {
}

void Poller::addFd(int fd, int events) {
    pollfd pollfd;
    pollfd.fd = fd;
    pollfd.events = events;
    pollfd.revents = 0;

    fdVector.push_back(pollfd);
}

void Poller::removeFd(int fd) {
    for (int i = 0; i < fdVector.size(); i++) {
        if (fdVector[i].fd == fd) {
            fdVector.erase(fdVector.begin() + i);
            break;
        }
    }
}

void Poller::_poll() {
    for (auto &i : fdVector)
        i.revents = 0;

    poll(fdVector.data(), fdVector.size(), -1);
}

std::vector<PollResult> Poller::getPollResult() {
    std::vector<PollResult> result;
    for (auto &i : fdVector) {
        if (i.revents != 0) {
            PollResult pollResult;
            pollResult.fd = i.fd;
            pollResult.revents = i.revents;

            result.push_back(pollResult);
        }
    }

    return result;
}
