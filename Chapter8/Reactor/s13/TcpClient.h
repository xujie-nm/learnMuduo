#ifndef MUDUO_NET_TCPCLIENT_H
#define MUDUO_NET_TCPCLIENT_H

#include <boost/noncopyable.hpp>

#include "thread/Mutex.h"
#include "TcpConnection.h"

namespace muduo{

    class Connector;
    typedef boost::shared_ptr<Connector> ConnectorPtr;

    /// 支持重连
    class TcpClient : boost::noncopyable{
        public:

            TcpClient(EventLoop* loop,
                      const InetAddress& serverAddr);
            ~TcpClient(); // force out-line dtor, for scoped_ptr member.

            void connect();
            void disconnect();
            void stop();

            TcpConnectionPtr connection() const{
                MutexLockGuard lock(mutex_);
                return connection_;
            }

            bool retry() const;
            void enableRetry(){
                retry_ = true;
            }

            // Set connection callback
            // Not thread safe.
            void setConnectionCallback(const ConnectionCallback& cb){
                connectionCallback_ = cb;
            }

            // Set message callback.
            // Not thread safe.
            void setMessageCallback(const MessageCallback& cb){
                messageCallback_ = cb;
            }

            // Set wriite complete callback.
            // Not thread safe.
            void setWriteCompleteCallback(const WriteCompleteCallback& cb){
                writeCompleteCallback_ = cb;
            }

        private:
            // Not thread safe, but in loop
            void newConnection(int sockfd);
            /// Not thread safe, but in loop
            void removeConnection(const TcpConnectionPtr& conn);

            EventLoop* loop_;
            ConnectorPtr connector_; // avoid revealing Connector
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            bool retry_;   // atmoic
            bool connect_; // atmoic
            // always in loop thread
            int nextConnId_;
            mutable MutexLock mutex_;
            TcpConnectionPtr connection_; // @BuardeBy mutex_
    };

}

#endif // MUDUO_NET_TCPCLIENT_H
