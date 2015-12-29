#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <boost/noncopyable.hpp>

namespace muduo{
    class InetAddress;

    ///
    /// Wrapper of socket file descriptor
    ///
    /// It closes the sockfd when desctructs.
    /// It's thread safe, all operation are delagated to OS.
    class Socket : boost::noncopyable{
        
        public:
            explicit Socket(int sockfd)
                : sockfd_(sockfd){
            }

            ~Socket();

            int fd() const {
                return sockfd_;
            }

            // abort if address in use
            void bindAddress(const InetAddress& localaddr);
            // abort if address inuse
            void listen();

            /// On success, return a non-negative integer that is
            /// a descriptor for the accepted socket, which has been
            /// set to non-blocking and close-on-exec. *peeraddr is addigned.
            /// On error, -1 is returned, and *peeraddr is untouched.
            int accept(InetAddress* peeraddr);

            /// 
            /// Enable/disable SO_REUSEADDR
            ///
            void setReuseAddr(bool on);

            void shutdownWrite();

        private:
            const int sockfd_;
    };
}

#endif // MUDUO_NET_SOCKET_H
