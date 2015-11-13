#include "daytime.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

// 建立连接后，返回当前系统时间，然后断开连接
int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(8888);
    DaytimeServer server(&loop, listenAddr);
    
    server.start();
    
    loop.loop();
    return 0;
}
