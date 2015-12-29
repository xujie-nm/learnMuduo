#include "TcpConnection.h"

#include "logging/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <errno.h>
#include <stdio.h>

using namespace muduo;

TcpConnection::TcpConnection(EventLoop* loop,
                             const std::string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
    : loop_(CHECK_NOTNULL(loop)),
      name_(nameArg),
      state_(kConnecting),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop, sockfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr){
    LOG_DEBUG << "TcpConnection::ctor[" << name_ << "] at " << this
              << " fd = " << sockfd;
    channel_->setReadCallback(
            boost::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(
            boost::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(
            boost::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(
            boost::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection(){
    LOG_DEBUG << "TcpConnection::dtor[" << name_ << "] at " << this
              << " fd= " << channel_->fd();
}

void TcpConnection::send(const std::string& message){
    if(state_ == kConnected){
        if(loop_->isInLoopThread()){
            sendInLoop(message);
        }else{
            // 保证在IO线程中调用,把message复制一份到IO线程中
            // 考虑使用移动语义来改进
            loop_->runInLoop(
                    boost::bind(&TcpConnection::sendInLoop, this, message));
        }
    }
}

void TcpConnection::sendInLoop(const std::string& message){
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    // if no thing in output queue, try writing directly
    // 这里是刚开始接收到数据写的时候
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0){
       nwrote  = ::write(channel_->fd(), message.data(), message.size());
       if(nwrote >= 0){
           if(implicit_cast<size_t>(nwrote) < message.size());
           LOG_TRACE << "I am going to write more data";
       }else{
           nwrote = 0;
           if(errno != EWOULDBLOCK){
               LOG_SYSERR << "TcpConnection::sendInLoop";
           }
       }
    }

    // 开始关注writable事件,以后的写数据就交给了handlewrite
    assert(nwrote >= 0);
    if(implicit_cast<size_t>(nwrote) < message.size()){
        outputBuffer_.append(message.data()+nwrote, message.size()-nwrote);
        if(!channel_->isWriting()){
            channel_->enableWriting();
        }
    }
}

void TcpConnection::shutdown(){
    // FIXME: use compare and swap
    if(state_ == kConnected){
        setState(kDisconnecting);
        // FIXME: shared_from_this()?
        loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop(){
    loop_->assertInLoopThread();
    if(!channel_->isWriting()){
        // we are not writing
        socket_->shutdownWrite();
    }
}

void TcpConnection::connectEstablished(){
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestoryed(){
    loop_->assertInLoopThread();
    assert(state_ == kConnected || state_ == kDisconnecting);
    setState(kDisconnected);
    channel_->disableAll();
    connectionCallback_(shared_from_this());

    loop_->removeChannel(get_pointer(channel_));
}

void TcpConnection::handleRead(Timestamp receiveTime){
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if(n > 0){
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }else if(n == 0){
        handleClose();
    }else{
        errno = savedErrno;
        LOG_SYSERR << "TcpConnection::handleRead";
        handleError();
    }
}

void TcpConnection::handleWrite(){
    loop_->assertInLoopThread();
    if(channel_->isWriting()){
        ssize_t n = ::write(channel_->fd(),
                            outputBuffer_.peek(),
                            outputBuffer_.readableBytes());
        if(n > 0){
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0){ // 发送完毕
                channel_->disableWriting();
                if(state_ == kDisconnecting){
                    shutdownInLoop();
                }
            }else{
                LOG_TRACE << "I am going to write more data";
            }
        }else{
            LOG_SYSERR << "TcpConnection::handleWrite";
            abort(); // FIXME
        }
    }else{
        LOG_TRACE << "Connection is down, no more writing";
    }
}

void TcpConnection::handleClose(){
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpConnection::handleClose state = " << state_;
    assert(state_ == kConnected || state_ == kDisconnecting);
    // we don't close fd, leave it to dtor, so we can find leaks easily.
    channel_->disableAll();
    // must be the last line
    closeCallback_(shared_from_this());
}

void TcpConnection::handleError(){
    int err = sockets::getSocketError(channel_->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_
              << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}
