#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include "thread/Thread.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace muduo{
    class Channel;
    class Poller;

    class EventLoop : boost::noncopyable{
        public:
            // 建立事件轮询时，需判断当前线程是否有事件轮询
            EventLoop();
            
            // force out-line dtor, for scoped_ptr members.
            ~EventLoop();

            ///
            /// Loops forever
            ///
            /// Must be called in the same thread as creation of the object.
            ///
            // 启动一个事件轮询
            // 启动时需要注意当前事件轮询是否启动
            // 当前事件轮询是否不在本线程中
            // 获得当前活动事件的Channel列表，
            // 依次调用每个Channel的handleEvent函数
            void loop();
            
            // 用于退出poll
            // 可能不会立即起效，
            // 因为是设标志
            // 可能当前还在loop中
            void quit();

            // internal use only
            // 通过调用Poller的updateChannel来实现
            void updateChannel(Channel* channel);
            // void removeChannel(Channel* channel);

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

            typedef std::vector<Channel*> ChannelList;

            bool looping_; // atomic
            bool quit_; // atomic
            const pid_t threadId_;
            boost::scoped_ptr<Poller> poller_;
            ChannelList activeChannels_;
    };
}

#endif    // MUDUO_NET__EVENTLOOP_H

