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

            ///
            /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
            /// Nagle算法是用来解决数据包过小频繁发送的问题,
            /// 通过把较小的数据包放到缓存里攒到一定数量在发送.
            /// 通过这种方法可以减轻网络的负荷
            /// 但是在低延迟网络服务中,这种方式会增加网络的延迟
            /// 所以在实现低延迟网络服务时,禁用Nagle算法
            ///
            void setTcpNoDelay(bool on);

        private:
            const int sockfd_;
    };
}

#endif // MUDUO_NET_SOCKET_H
