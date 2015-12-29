#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Channel.h"
#include "Socket.h"

namespace muduo{
    
    class EventLoop;
    class InetAddress;

    ///
    /// Acceptor of incoming TCP connection.
    ///
    class Acceptor : boost::noncopyable{
        
        public:
            typedef boost::function<void (int sockfd,
                                          const InetAddress&)> NewConnectionCallback;

            Acceptor(EventLoop* loop, const InetAddress& listenAddr);

            void setNewConnectionCallback(const NewConnectionCallback& cb){
                newConnectionCallback_ = cb;
            }

            bool listening() const{
                return listening_;
            }

            void listen();

        private:

            void handleRead();

            EventLoop* loop_;
            Socket acceptSocket_;
            Channel acceptChannel_;
            NewConnectionCallback newConnectionCallback_;
            bool listening_;
    };

}

#endif // MUDUO_NET_ACCEPTOR_H
