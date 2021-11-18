cmake_minimum_required(VERSION 3.17)
project(ppr)

set(CMAKE_CXX_STANDARD 20)

add_executable(ppr
        Cache.cpp
        CacheRepository.cpp
        ClientSocketHandler.cpp
        DestServerSocketHandler.cpp
        ProxyServer.cpp
        main.cpp
        Poll_Impl.cpp
        ProxyServerSocket.cpp
        TcpSocket.cpp)
