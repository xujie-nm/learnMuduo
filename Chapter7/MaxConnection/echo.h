#ifndef LEARNMUDUO_CHPATER7_MAXCONNECTION_ECHO_H
#define LEARNMUDUO_CHPATER7_MAXCONNECTION_ECHO_H

#include <muduo/net/TcpServer.h>

class EchoServer{
    public:
        EchoServer(muduo::net::EventLoop* loop,
                   const muduo::net::InetAddress& listenAddr,
                   int maxConnections);

        void start();

    private:

        void onConnection(const muduo::net::TcpConnectionPtr& conn);

        void onMessage(const muduo::net::TcpConnectionPtr& conn,
                          muduo::net::Buffer* buf,
                          muduo::Timestamp time);

        muduo::net::TcpServer server_;
        // 如果是多线程的话，这里的类型atomic_int
        // 因为可能有竞态条件
        int numConnected_;
        const int kMaxConnections_;
};

#endif
