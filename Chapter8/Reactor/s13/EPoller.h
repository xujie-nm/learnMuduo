#ifndef MUDUO_NET_EPOLLER_H
#define MUDUO_NET_EPOLLER_H

#include <map>
#include <vector>

#include "datetime/Timestamp.h"
#include "EventLoop.h"

// the defination of epoll_event
// typedef union epoll_data{
//     void     *ptr;
//     int      fd;
//     uint32_t u32;
//     uint64_t u64;
// } epoll_data_t;
//
// struct epoll_event{
//     uint32_t     events;     /* Epoll events */
//     epoll_data_t data;       /* User data variable */
// };
//
// 注意:epoll_data是一个union,muduo使用的是其ptr成员,
// 用于存放Channel*, 这样可以减少一步查询.

struct epoll_event;

namespace muduo{

class Channel;

///
/// IO Multiplexing with epoll(4)
/// Linux独有的高效事件轮询机制
/// 高效的地方在于poll(2)每次轮询返回的是整个文件描述符数组,
/// 用户代码遍历数组以找到那些文件描述符上有IO事件.
/// 而epoll_wait(2)返回的是活动的文件描述符列表, 
/// 需要遍历的数组通常会小很多
/// 
/// This class doesn't own the Channel objects.
class EPoller : boost::noncopyable{
    public:
        typedef std::vector<Channel*> ChannelList;

        EPoller(EventLoop* loop);
        ~EPoller();

        /// Polls the I/O events.
        /// Must be called in the loop thread.
        Timestamp poll(int timeoutMs, ChannelList* activeChannels);

        /// 这两个updateChannel()和removeChannel()
        /// 是由于epoll是有状态的,所以使用这
        /// 两个函数来维护内核中的fd状态和应
        /// 用程序中的状态相符合
        /// Changes the interested I/O events.
        /// Must be called in the loop thread.
        void updateChannel(Channel* channel);
        /// Remove the channel, when it destructs.
        /// Must be called in the loop thread.
        void removeChannel(Channel* channel);

        void assertInLoopThread(){
            ownerLoop_->assertInLoopThread();
        }

    private:
        static const int kInitEventListSize = 16;

        // 将events_中活动的fd填入ActiveChannels
        void fillActiveChannels(int numEvents,
                                ChannelList* activeChannels) const;
        void update(int operation, Channel* channel);

        typedef std::vector<struct epoll_event> EventList;
        typedef std::map<int, Channel*> ChannelMap;

        EventLoop* ownerLoop_;
        int epollfd_;
        // 保存的是活动的fd列表,它的大小是自适应的
        EventList events_;
        ChannelMap channels_;
};

}

#endif  // MUDUO_NET_EPOLLER_H
