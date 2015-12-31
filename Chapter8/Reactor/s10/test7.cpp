#include "datetime/Timestamp.h"

#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>
#include <string>

void newConnection9981(int sockfd, const muduo::InetAddress& peerAddr){
    printf("newConnection9981(): accepted a new connection from %s\n",
            peerAddr.toHostPort().c_str());
    std::string time = muduo::Timestamp::now().toFormattedString() + "\n";
    ::write(sockfd, time.c_str(), time.size());
    muduo::sockets::close(sockfd);
}

void newConnection8888(int sockfd, const muduo::InetAddress& peerAddr){
    printf("newConnection8888(): accepted a new connection from %s\n",
            peerAddr.toHostPort().c_str());
    std::string time = muduo::Timestamp::now().toString() + "\n";
    ::write(sockfd, time.c_str(), time.size());
    muduo::sockets::close(sockfd);
}

int main(int argc, const char *argv[])
{
    printf("main(): pid = %d\n", getpid());

    muduo::InetAddress listenAddr(9981);
    muduo::InetAddress listenAddr1(8888);
    muduo::EventLoop loop;

    muduo::Acceptor acceptor(&loop, listenAddr);
    acceptor.setNewConnectionCallback(newConnection9981);
    acceptor.listen();
    
    muduo::Acceptor acceptor1(&loop, listenAddr1);
    acceptor1.setNewConnectionCallback(newConnection8888);
    acceptor1.listen();

    loop.loop();
    return 0;
}
