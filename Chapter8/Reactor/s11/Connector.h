#ifndef MUDUO_NET_CONNECTOR_H
#define MUDUO_NET_CONNECTOR_H

#include "InetAddress.h"
#include "TimerId.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace muduo{
    
    class Channel;
    class EventLoop;

    class Connector : boost::noncopyable{
        
        public: 
            typedef boost::function<void (int sockfd)> NewConnectionCallback;

            Connector(EventLoop* loop, const InetAddress& serverAddr);
            ~Connector();

            void setNewConnectionCallback(const NewConnectionCallback& cb){
                newConnectionCallback_ = cb;
            }

            void start(); // can be called in any thread
            void restart(); // must be called in loop thread
            void stop(); // can be called in any thread

            const InetAddress& serverAddress() const{
                return serverAddr_;
            }

        private:
            enum States { kDisconnected, kConnecting, kConnected };
            static const int kMaxRetryDelayMs = 30*1000;
            static const int kInitRetryDelayMs = 500;

            void setState(States s){
                state_ = s;
            }
            // 供start,restart,retry调用
            void startInLoop();
            // 供startInLoop调用
            void connect();
            // 供connect调用
            void connecting(int sockfd);
            // 供connecting调用
            void handleWrite();
            // 供connecting调用
            void handleError();
            // 供connect,handleWrite,handleError调用
            void retry(int sockfd);
            int removeAndResetChannel();
            void resetChannel();

            EventLoop* loop_;
            InetAddress serverAddr_;
            bool connect_; // atomic
            States state_; // FIXME: use atomic variable
            boost::scoped_ptr<Channel> channel_;
            NewConnectionCallback newConnectionCallback_;
            int retryDelayMs_;
            TimerId timerId_;
    };
    typedef boost::shared_ptr<Connector> ConnectorPtr;

}

#endif  // MUDUO_NET_CONNECTOR_H
