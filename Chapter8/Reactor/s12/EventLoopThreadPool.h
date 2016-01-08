#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include "thread/Condition.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"

#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muduo{
    
    class EventLoop;
    class EventLoopThread;

    /// 从线程池中挑选一个loop给TcpConnection
    /// 也就是TcpServer自己的EventLoop只用来接受新连接
    /// 而新连接会用其他EventLoop来执行IO
    class EventLoopThreadPool : boost::noncopyable{
        public:
            EventLoopThreadPool(EventLoop* baseLoop);
            ~EventLoopThreadPool();
            
            void setThreadNum(int numThreads){
                numThreads_ = numThreads;
            }

            void start();
            // 接受新连接用这个分配loop
            EventLoop* getNextLoop();

        private:
            EventLoop* baseLoop_;
            bool started_;
            int numThreads_;
            int next_; // always in loop thread
            boost::ptr_vector<EventLoopThread> threads_;
            std::vector<EventLoop*> loops_;
    };

}

#endif // MUDUO_NET_EVENTLOOPTHREADPOOL_H
