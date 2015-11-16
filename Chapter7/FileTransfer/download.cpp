#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

const char* g_file = NULL;

string readFile(const char* filename){
    string content;
    FILE* fp = ::fopen(filename, "rb");
    if(fp){
        const int kBufSize = 1024*1024;
        char iobuf[kBufSize];
        ::setbuffer(fp, iobuf, sizeof(iobuf));

        char buf[kBufSize];
        size_t nread = 0;
        while((nread = ::fread(buf, 1, sizeof(buf), fp)) > 0){
            content.append(buf, nread);
        }
        ::fclose(fp);
    }
    return content;
}

void onHighWaterMark(const TcpConnectionPtr& conn, size_t len){
    LOG_INFO << "HighWaterMark " << len;
}

void onConnection(const TcpConnectionPtr& conn){
    LOG_INFO << "FileServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is " 
             << (conn->connected() ? "UP" : "DOWN");
    if(conn->connected()){
        LOG_INFO << "FileServer - Sending file " << g_file
                 << " to " << conn->peerAddress().toIpPort();
        conn->setHighWaterMarkCallback(onHighWaterMark, 64*1024);
        string fileContent = readFile(g_file);
        conn->send(fileContent);
        conn->shutdown();
        LOG_INFO << "FileServer - done";
    }
}

// 一次性把文件读入内存
// 内存消耗不仅仅与并发连接数有关，还和文件大小有关
// 所以此程序的健壮性不够
int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    if(argc > 1){
        g_file = argv[1];

        EventLoop loop;
        InetAddress listenAddr(8888);
        TcpServer server(&loop, listenAddr, "FileServer");
        server.setConnectionCallback(onConnection);
        server.start();
        loop.loop();
    }else{
        fprintf(stderr, "Usage: %s file_for_downloading\n", argv[0]);
    }
    return 0;
}
