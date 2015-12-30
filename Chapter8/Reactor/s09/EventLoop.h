#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include "datetime/Timestamp.h"
#include "thread/Mutex.h"
#include "thread/Thread.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace muduo{
    class Channel;
    class Poller;
    class TimerQueue;

    class EventLoop : boost::noncopyable{
        public:

            typedef boost::function<void()> Functor;

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

            ///
            /// Time when poll returns, usually means data arrivial.
            ///
            Timestamp pollReturnTime() const{
                return pollReturnTime_;
            }

            // Runs callback immediately in the loop thread
            // It wakes up the loop, and run the cb
            // If in the same loop threadm cb is run within the function.
            // Safe to call from other threads.
            // 在IO线程内执行某个一用户任务回调
            void runInLoop(const Functor& cb);

            // Queues callback in the loop thread.
            // Runs after finish pooling.
            // Safe to call from other threads.
            void queueInLoop(const Functor& cb);

            // timers

            ///
            /// Runs callback at 'time'.
            ///
            TimerId runAt(const Timestamp& time, const TimerCallback& cb);
            ///
            /// Runs callback after @c delay seconds
            ///
            TimerId runAfter(double delay, const TimerCallback& cb);
            ///
            /// Runs callback every @c interval seconds.
            ///
            TimerId runEvery(double interval, const TimerCallback& cb);


            // void cancel(TimerId timerId)

            // internal use only

            //　唤醒IO线程
            void wakeup();

            void updateChannel(Channel* channel);
            // 通过调用Poller的updateChannel来实现
            void removeChannel(Channel* channel);

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
            void handleRead(); // wake up
            void doPendingFunctors();

            typedef std::vector<Channel*> ChannelList;

            bool looping_; // atomic
            bool quit_; // atomic
            bool callingPendingFunctors_; // atomic
            const pid_t threadId_;
            Timestamp pollReturnTime_;
            boost::scoped_ptr<Poller> poller_;
            boost::scoped_ptr<TimerQueue> timerQueue_;
            int wakeupFd_;
            // unlike in TimerQueue, which is an internal class,
            // we don't expose Channel to Client.
            // 用于处理wakeupFd_上的readable事件
            // 将事件分发至handleRead()
            boost::scoped_ptr<Channel> wakeupChannel_;
            ChannelList activeChannels_;
            MutexLock mutex_;
            std::vector<Functor> pendingFunctors_; // @GuardedBy mutex_
    };
}

#endif    // MUDUO_NET__EVENTLOOP_H

