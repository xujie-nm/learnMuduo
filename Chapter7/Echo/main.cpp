#include "echo.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(8888);
    EchoServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}
