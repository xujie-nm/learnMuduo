#include "EventLoop.h"
#include "thread/Thread.h"

muduo::EventLoop* g_loop;

void threadFunc(){
    g_loop->loop();
}

// 这个测试尝试在新建的线程中调用ＩＯ线程中的事件轮询
// 遵循one loop per thread选择，程序中断
int main(int argc, const char *argv[])
{
    muduo::EventLoop loop;
    g_loop = &loop;
    muduo::Thread t(threadFunc);
    t.start();
    t.join();
    return 0;
}
