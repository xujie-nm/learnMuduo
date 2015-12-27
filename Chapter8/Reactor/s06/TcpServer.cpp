#include "TcpServer.h"

#include "logging/Logging.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <stdio.h> // snprintf

using namespace muduo;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string name)
    : loop_(CHECK_NOTNULL(loop)),
      name_(name),
      acceptor_(new Acceptor(loop, listenAddr)),
      started_(false),
      nextConnId_(1){
    acceptor_->setNewConnectionCallback(
            boost::bind(&TcpServer::newConnection, this, _1, _2));    
}

TcpServer::~TcpServer(){
}

void TcpServer::start(){
    if(!started_){
        started_ = true;
    }

    if(!acceptor_->listening()){
        loop_->runInLoop(
                boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr){
    loop_->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof buf, "#%d", nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    LOG_INFO << "TcpServer::newConnection [" << name_
             << "] - new connection [" << connName
             << "] from" << peerAddr.toHostPort();
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    TcpConnectionPtr conn(
            new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallback(
            boost::bind(&TcpServer::removeConnection, this, _1));
    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn){
    loop_->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnection [" << name_
             << "] - connection " << conn->name();
    size_t n = connections_.erase(conn->name());
    assert(n == 1);
    (void) n;
    // 延长生命周期,让conn的生命周期延长到connectedDestoryed被调用的时候
    loop_->queueInLoop(
            boost::bind(&TcpConnection::connectDestoryed, conn));
}
