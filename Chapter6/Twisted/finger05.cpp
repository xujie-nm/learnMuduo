// 读取用户名，输出错误信息，然后断开连接
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

using namespace muduo;
using namespace muduo::net;

void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp receiveTime){
    if(buf->findCRLF()){
        conn->send("No such user\r\n");
        conn->shutdown();
    }
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(8888), "Finger");
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
    return 0;
}
