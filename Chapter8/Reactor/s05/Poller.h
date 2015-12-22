#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include <map>
#include <vector>

#include "datetime/Timestamp.h"
#include "EventLoop.h"

struct pollfd;

namespace muduo{
    class Channel;

    ///
    /// IO Multiplexing with poll(2)
    ///
    /// This class doesn't own the Channel objects
    class Poller : boost::noncopyable{
        public:
            typedef std::vector<Channel*> ChannelList;

            Poller(EventLoop* loop);
            virtual ~Poller();

            /// Polls the I/O events
            /// Must be called in the loop thread.
            Timestamp poll(int timeoutMs, ChannelList* activeChannels);

            /// Changes the interested I/O events
            /// Must be called in the loop thread.
            /// 更新维护pollfds数组
            void updateChannel(Channel* channel);

            void assertInLoopThread(){
                ownerLoop_->assertInLoopThread();
            }

        private:
           // 遍历pollfds找出有活动事件的fd，把对应的channel填入activechannels
           void fillActiveChannels(int numEvents,
                                   ChannelList* activeChannels) const;

           typedef std::vector<struct pollfd> PollFdList;
           typedef std::map<int, Channel*> ChannelMap;
            
           // 所属loop
           EventLoop* ownerLoop_;
           // 把pollfds缓存起来
           PollFdList pollfds_;
           //　存储fd到Channel*的映射
           ChannelMap channels_;
    };
}

#endif // MUDUO_NET_POLLER_H
