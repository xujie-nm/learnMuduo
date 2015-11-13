#include "discard.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

// 和客户端连接后丢弃所有数据
int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(8888);
    DiscardServer server(&loop, listenAddr);
    
    server.start();

    loop.loop();
    return 0;
}
