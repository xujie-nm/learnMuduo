#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include "datetime/Timestamp.h"
#include "thread/Mutex.h"
#include "Callbacks.h"
#include "Channel.h"

namespace muduo{
    class EventLoop;
    class Timer;
    class TimerId;

    ///
    /// A best efforts timer queue.
    /// No guarantee that the callback will be on time.
    ///
    /// 高效的组织当前尚未到期的Timer
    class TimerQueue : boost::noncopyable{
        public:
            TimerQueue(EventLoop* loop);
            ~TimerQueue();

            ///
            /// Schedules the callback to be run at given time,
            /// repeats if @c interval
            /// 
            /// Must be thread safe, Usually be called from other threads.
            /// 供EventLoop使用
            TimerId addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval);

            // void cancel(TimerId timerId);

        private:

            // FIXME: use unique_ptr<Timer> instead of raw pointers.
            typedef std::pair<Timestamp, Timer* > Entry;
            typedef std::set<Entry> TimerList;

            // called when timerfd alarms
            void handleRead();
            // move out all expired timers
            std::vector<Entry> getExpired(Timestamp now);
            void reset(const std::vector<Entry>& expired, Timestamp now);

            bool insert(Timer* timer);

            EventLoop* loop_;
            const int timerfd_;
            Channel timerfdChannel_;
            // Timer list sorted by expiration
            TimerList timers_;
    };
}

#endif // MUDUO_NET_TIMESTAMP_H
