#include "EventLoop.h"

#include <boost/bind.hpp>

#include <stdio.h>

int cnt = 0;
muduo::EventLoop* g_loop;

void printTid(){
    printf("pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
    printf("now %s\n", muduo::Timestamp::now().toString().c_str());
}

void print(const char* msg){
    printf("msg %s %s\n", muduo::Timestamp::now().toString().c_str(), msg);
    if(++cnt == 20){
        g_loop->quit();
    }
}

int main(int argc, const char *argv[])
{
    printTid();
    muduo::EventLoop loop;
    g_loop = &loop;

    print("main");
    loop.runAfter(1, boost::bind(printTid));
    loop.runAfter(1.5, boost::bind(printTid));
    loop.runAfter(2.5, boost::bind(printTid));
    loop.runAfter(3.5, boost::bind(printTid));
    loop.runAfter(2, boost::bind(printTid));
    loop.runAfter(3, boost::bind(printTid));
    loop.runEvery(3, boost::bind(print, "233333333"));

    loop.loop();
    print("main loop exists");
    sleep(1);
    return 0;
}
