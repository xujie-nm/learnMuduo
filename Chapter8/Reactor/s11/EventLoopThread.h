#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include "thread/Condition.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"

#include <boost/noncopyable.hpp>

namespace muduo{
    class EventLoop;

    // 启动自己的线程，并在其中运行loop
    class EventLoopThread : boost::noncopyable{
        public:

            EventLoopThread();
            ~EventLoopThread();
            EventLoop* startLoop();

        private:

            void threadFunc();

            EventLoop* loop_;
            bool exiting_;
            Thread thread_;
            MutexLock mutex_;
            Condition cond_;
    };
}

#endif
