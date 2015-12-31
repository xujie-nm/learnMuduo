#include "Channel.h"
#include "EventLoop.h"

#include <stdio.h>
#include <sys/timerfd.h>

muduo::EventLoop* g_loop;

void timeout(muduo::Timestamp receiveTime){
    printf("%s Timeout! go go go!!!!\n", receiveTime.toFormattedString().c_str());
    g_loop->quit();
}

// 用timerfd实现一个单次出发定时器
// 利用Channel将timerfd的readable事件转发给timeout函数
int main(int argc, const char *argv[])
{
    printf("%s started\n", muduo::Timestamp::now().toFormattedString().c_str());
    muduo::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 3;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
    return 0;
}
