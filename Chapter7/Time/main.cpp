#include "time.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

// 以二进制的形式发送当前时间，然后断开连接
int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(8888);
    TimeServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}
