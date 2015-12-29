#ifndef MUDUO_NET_TCPSERVER_H
#define MUDUO_NET_TCPSERVER_H

#include "Callbacks.h"
#include "TcpConnection.h"

#include <map>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo{

    class Acceptor;
    class EventLoop;

    // 该类的功能是管理接收到的TcpConnection
    class TcpServer : boost::noncopyable{
        public:

            TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string name);
            ~TcpServer();   // force out-line dtor, for scoped_ptr members.

            /// Starts the server if it's not listening.
            /// 
            /// It's harmless to call it multiple times.
            /// Thread safe.
            void start();

            /// Set connection callback.
            /// Not thread safe.
            void setConnectionCallback(const ConnectionCallback& cb){
                connectionCallback_ = cb;
            }

            /// Set message callback.
            /// Not thread safe.
            void setMessageCallback(const MessageCallback& cb){
                messageCallback_ = cb;
            }

        private:
            /// Not thread safe, but in loop
            void newConnection(int sockfd, const InetAddress& peerAddr);
            void removeConnection(const TcpConnectionPtr& conn);

            typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

            EventLoop* loop_; // the acceptor loop
            const std::string name_;
            boost::scoped_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            bool started_;
            int nextConnId_; // always in loop thread
            ConnectionMap connections_;
    };

}

#endif // MUDUO_NET_TCPSERVER_H
