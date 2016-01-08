#include "EventLoop.h"

// 同样遵循one loop per thread原则
// 一个ＩＯ线程中不能有两个事件轮询
int main(int argc, const char *argv[])
{
    muduo::EventLoop loop1;
    muduo::EventLoop loop2;

    loop1.loop();
    loop2.loop();
    return 0;
}
