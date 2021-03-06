#include "ProxyServer.h"

ProxyServer::ProxyServer(int port) : serverSocket(port) {
    cache = new Cache();

    poller.addToPoll(serverSocket.getFd(), POLLIN | POLLPRI);
}

ProxyServer::~ProxyServer() = default;

void ProxyServer::acceptClient() {
    auto *clientSocket = new TcpSocket(serverSocket.socketAccept());

    poller.addToPoll(clientSocket->getFd(), POLLIN | POLLHUP | POLLOUT);

    ProxyTunnel proxyEntry;
    proxyEntry.cacheReader = new CacheRepository(cache, clientSocket);
    proxyEntry.clientSocket = clientSocket;
    proxyEntry.clientSocketHandler = new ClientSocketHandler(clientSocket, this);
    proxyEntry.remoteServerSocket = nullptr;
    proxyEntry.remoteServerSocketHandler = nullptr;

    proxyTunnels.push_back(proxyEntry);
}

[[noreturn]] void ProxyServer::run() {
    while (true) {
        poller.makePoll();
        auto pollResults = poller.getPollResult();

        for (auto &pollResult : pollResults) {
            if (pollResult.fd == serverSocket.getFd() && pollResult.fd & (POLLIN | POLLPRI)) {
                acceptClient();

            } else {
                for (int i = 0; i < proxyTunnels.size(); i++) {
                    ProxyTunnel proxyTunnel = proxyTunnels[i];

                    if (proxyTunnel.clientSocket->getFd() == pollResult.fd ||
                        (proxyTunnel.remoteServerSocket != nullptr &&
                         proxyTunnel.remoteServerSocket->getFd() == pollResult.fd)) {

                        bool isKeepConnection = proxyTunnel.clientSocketHandler->handlePollResult(pollResult);

                        if (!isKeepConnection) {
                            closeConnection(i);
                            continue;
                        }

                        if (proxyTunnel.remoteServerSocket != nullptr) {
                            isKeepConnection = proxyTunnel.remoteServerSocketHandler->handlePollRevents(pollResult);

                            if (!isKeepConnection) {
                                closeConnection(i);
                                continue;
                            }
                        }

                        if (proxyTunnel.cacheReader->isReading()) {
                            isKeepConnection = proxyTunnel.cacheReader->handlePollRevents(pollResult);

                            if (!isKeepConnection) {
                                closeConnection(i);
                                continue;
                            }
                        }

                    }
                }
            }
        }
    }
}

void ProxyServer::closeConnection(int numberOfConnection) {
    ProxyTunnel proxyEntry = proxyTunnels[numberOfConnection];

    poller.removeFromPoll(proxyEntry.clientSocket->getFd());
    proxyEntry.clientSocket->closeSocket();
    delete proxyEntry.clientSocket;
    delete proxyEntry.clientSocketHandler;

    if (proxyEntry.remoteServerSocket != nullptr) {
        poller.removeFromPoll(proxyEntry.remoteServerSocket->getFd());
        proxyEntry.remoteServerSocket->closeSocket();
        delete proxyEntry.remoteServerSocket;

        proxyEntry.remoteServerSocketHandler->endReadingResponce();
        delete proxyEntry.remoteServerSocketHandler;
    }

    if (proxyEntry.cacheReader->isReading()) {
        proxyEntry.cacheReader->finishRead();
    }

    delete proxyEntry.cacheReader;

    proxyTunnels.erase(proxyTunnels.begin() + numberOfConnection);
}

void ProxyServer::newRequest(ClientSocketHandler *clientSocketHandler, char *url) {
    if (!cache->containsRecord(url)) {
        for (ProxyTunnel &proxyTunnel : proxyTunnels) {
            if (proxyTunnel.clientSocketHandler == clientSocketHandler) {
                long hostUrlLen = strstr(url, "/") - url;
                char hostUrl[hostUrlLen + 1];
                memccpy(hostUrl, url, 0, hostUrlLen);
                hostUrl[hostUrlLen] = '\0';

                if (proxyTunnel.remoteServerSocket == nullptr) {
                    proxyTunnel.remoteServerSocket = new TcpSocket();
                    proxyTunnel.remoteServerSocket->socketConnect(hostUrl, DefaultDestPort);

                    poller.addToPoll(proxyTunnel.remoteServerSocket->getFd(), POLLIN | POLLHUP | POLLOUT);

                    clientSocketHandler->setDestServerSocket(proxyTunnel.remoteServerSocket);

                    proxyTunnel.remoteServerSocketHandler = new DestServerSocketHandler(proxyTunnel.clientSocket,
                                                                                        proxyTunnel.remoteServerSocket,
                                                                                        cache);
                } else if (strcmp(hostUrl, proxyTunnel.remoteServerSocket->getHostName()) != 0) {
                    int invalidDescriptor = proxyTunnel.remoteServerSocket->getFd();
                    proxyTunnel.remoteServerSocket->closeSocket();
                    poller.removeFromPoll(invalidDescriptor);
                    delete proxyTunnel.remoteServerSocket;

                    proxyTunnel.remoteServerSocket = new TcpSocket();
                    proxyTunnel.remoteServerSocket->socketConnect(hostUrl, DefaultDestPort);
                    poller.addToPoll(proxyTunnel.remoteServerSocket->getFd(), POLLIN | POLLHUP | POLLOUT);

                    clientSocketHandler->setDestServerSocket(proxyTunnel.remoteServerSocket);
                }

                if (proxyTunnel.cacheReader->isReading())
                    proxyTunnel.cacheReader->finishRead();
                else {
                    proxyTunnel.remoteServerSocketHandler->endReadingResponce();
                    proxyTunnel.remoteServerSocketHandler->waitForNextResponce(url);
                }
                break;
            }
        }
    } else {
        std::cout << "Data of " << url << " is in cache" << std::endl;

        for (ProxyTunnel &proxyEntry : proxyTunnels) {
            if (proxyEntry.clientSocketHandler == clientSocketHandler) {
                proxyEntry.cacheReader->read(url);
            }
        }

    }
}
