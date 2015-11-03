// 主动断开连接
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn){
    if(conn->connected())
        conn->shutdown();
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(8888), "Finger");
    server.setConnectionCallback(onConnection);
    server.start();
    loop.loop();
    return 0;
}
