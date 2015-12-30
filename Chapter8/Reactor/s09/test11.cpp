#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include <stdio.h>

std::string message;

void onConnection(const muduo::TcpConnectionPtr& conn){
    if(conn->connected()){
        printf("onConnection(): new connection [%s] from %s\n",
               conn->name().c_str(),
               conn->peerAddress().toHostPort().c_str());
        conn->send(message);
    }else{
        printf("onConnection(): connection [%s] is down\n",
               conn->name().c_str());
    }
}

// 每次发送完毕都要调用这个函数,死循环
void onWriteComplete(const muduo::TcpConnectionPtr& conn){
    conn->send(message);
}

void onMessage(const muduo::TcpConnectionPtr& conn,
               muduo::Buffer* buf,
               muduo::Timestamp receiveTime){
    printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
           buf->readableBytes(),
           conn->name().c_str(),
           receiveTime.toFormattedString().c_str());

    buf->retrieveAll();
}

int main(int argc, const char *argv[])
{
    printf("amin(): pid = %d\n", getpid());

    std::string line;
    for (int i = 33; i < 127; i++) {
        line.push_back(char(i));
    }
    line += line;

    for (size_t i = 0; i < 127-33; i++) {
        message += line.substr(i, 72) + '\n';
    }

    std::string serverName = "testServer";
    muduo::InetAddress listenAddr(9981);
    muduo::EventLoop loop;

    muduo::TcpServer server(&loop, listenAddr, serverName);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setWriteCompleteCallback(onWriteComplete);
    server.start();

    loop.loop();
    return 0;
}
