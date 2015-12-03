#include "resolver.h"
#include <muduo/net/EventLoop.h>
#include <boost/bind.hpp>
#include <stdio.h>
#include <vector>

using namespace muduo;
using namespace muduo::net;
using namespace cdns;

EventLoop* g_loop;
int count = 0;
int total = 0;

void quit(){
    g_loop->quit();
}

void resolveCallback(const string& host, const std::vector<InetAddress>& addrList){
    // change
    printf("resolveCallback %s\n", host.c_str());
    for (size_t i = 0; i < addrList.size(); i++) {
        printf("                   -> %s\n", addrList[i].toIpPort().c_str());
    }
    // end change
    if(++count == total)
        quit();
}

void resolve(Resolver* res, const string& host){
    // 这里注意resolve函数接收的callback的类型
    res->resolve(host, boost::bind(&resolveCallback, host, _1));
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    loop.runAfter(10, quit);
    g_loop = &loop;
    Resolver resolver(&loop,
            argc == 1 ? Resolver::kDNSonly : Resolver::kDNSandHostsFile);

    if(argc == 1){
        total = 3;
        resolve(&resolver, "www.github.com");
        resolve(&resolver, "www.bing.com");
        resolve(&resolver, "www.google.com");
    }else{
        total = argc-1;
        for (int i = 1; i < argc; i++) {
            resolve(&resolver, argv[i]);
        }
    }
    loop.loop();
    return 0;
}
