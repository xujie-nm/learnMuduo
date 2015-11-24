#include "echo.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(8888);
    int maxConnections = 5;
    if(argc > 1){
        maxConnections = atoi(argv[1]);
    }
    LOG_INFO << "maxConnection = " << maxConnections;
    EchoServer server(&loop, listenAddr, maxConnections);
    server.start();

    loop.loop();
    return 0;
}
