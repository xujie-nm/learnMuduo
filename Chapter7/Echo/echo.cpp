#include "echo.h"

#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

#include <stdlib.h>

EchoServer::EchoServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
    : server_(loop, listenAddr, "EchoServer"){
    
    server_.setConnectionCallback(
            boost::bind(&EchoServer::onConnection, this, _1));
    server_.setMessageCallback(
            boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start(){
    server_.start();
}

void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn){
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time){
    muduo::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
             << " data receive at " << time.toString();
    //changeWord(msg);
    //rot13(msg);
    conn->send(msg);
}

void EchoServer::changeWord(muduo::string& msg){
    for (size_t i = 0; i < msg.size(); i++) {
        if(islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if(isupper(msg[i]))
            msg[i] = tolower(msg[i]);
    }
}

void EchoServer::rot13(muduo::string& msg){
    for (size_t i = 0; i < msg.size(); i++) {
        if(isalpha(msg[i])){
            if(islower(msg[i]))
                msg[i] = (msg[i] - 'a' + 13) % 26 + 'a';
            else
                msg[i] = (msg[i] - 'A' + 13) % 26 + 'A';
        }
    }
}
