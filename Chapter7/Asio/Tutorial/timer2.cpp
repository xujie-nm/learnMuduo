#include <muduo/net/EventLoop.h>

#include <iostream>

void print(){
    std::cout << "Hello, world!\n";
}

// 非阻塞定时
int main(int argc, const char *argv[])
{
    muduo::net::EventLoop loop;
    loop.runAfter(5, print);
    
    loop.loop();
    return 0;
}
