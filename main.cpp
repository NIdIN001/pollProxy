#include "ProxyServer.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You must enter port" << std::endl;
        return 0;
    }

    try {
        auto *proxyServer = new ProxyServer(std::stoi(argv[1]));
        proxyServer->run();
    } catch (std::invalid_argument &exception) {
        std::cerr << "Port must be an integer value" << std::endl;
    }

    return 0;
}
