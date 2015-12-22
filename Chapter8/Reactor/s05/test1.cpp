#include "EventLoop.h"
#include "thread/Thread.h"
#include <stdio.h>

void threadFunc(){
    printf("threadFunc(): pid = %d, tid = %d\n",
            getpid(), muduo::CurrentThread::tid());

    muduo::EventLoop loop;
    //　在新建的线程中建立一个事件轮询
    loop.loop();
}

int main(int argc, const char *argv[])
{
    printf("main(): pid = %d, tid = %d\n",
            getpid(), muduo::CurrentThread::tid());

    muduo::EventLoop loop;
    // 在当前线程建立一个事件轮询

    muduo::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    pthread_exit(NULL);
    return 0;
}
