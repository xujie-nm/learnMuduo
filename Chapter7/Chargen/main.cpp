#include "chargen.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

// 接受连接后，不停的发送测试数据
int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(8888);
    ChargenServer server(&loop, listenAddr, true);
    server.start();
    loop.loop();
    return 0;
}
