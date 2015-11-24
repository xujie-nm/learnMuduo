#include <muduo/net/EventLoop.h>

#include <iostream>
#include <boost/bind.hpp>

void print(muduo::net::EventLoop* loop, int *count){
    if(*count < 5){
        std::cout << *count << "\n";
        ++(*count);

        loop->runAfter(1, boost::bind(print, loop, count));
    }else
        loop->quit();
}

// 在TimerCallback传递参数，像runevery
int main(int argc, const char *argv[])
{
    muduo::net::EventLoop loop;
    int count = 0;
    
    loop.runAfter(1, boost::bind(print, &loop, &count));

    loop.loop();
    std::cout << "Final count is " << count << "\n";
    return 0;
}
