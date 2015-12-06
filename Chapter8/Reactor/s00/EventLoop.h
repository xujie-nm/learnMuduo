#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include "thread/Thread.h"

namespace muduo{
    class EventLoop : boost::noncopyable{
        public:
            EventLoop();
            // 建立事件轮询时，需判断当前线程是否有事件轮询
            ~EventLoop();

            void loop();
            // 启动一个事件轮询
            // 启动时需要注意当前事件轮询是否启动
            // 当前事件轮询是否不在本线程中

            void assertInLoopThread(){
            //　断言是否在本线程中
                if(!isInLoopThread()){
                    abortNotInLoopThread();
                }
            }

            bool isInLoopThread()const{
            // 判断事件轮询存储的线程ＩＤ是否等于当前线程ＩＤ
                return threadId_ == CurrentThread::tid();
            }

        private:

            void abortNotInLoopThread();

            bool looping_; // atomic
            const pid_t threadId_;
    };
}

#endif    // MUDUO_NET__EVENTLOOP_H

