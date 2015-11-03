// 从空的UserMap里查找用户
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <map>

using namespace muduo;
using namespace muduo::net;

typedef std::map<string, string> UserMap;
UserMap users;

string getUser(const string& user){
    string result = "No such user";
    UserMap::iterator it = users.find(user);
    if(it != users.end()){
        result = it->second;
    }
    return result;
}

void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp receiveTime){
    const char* crlf = buf->findCRLF();
    if(crlf){
        string user(buf->peek(), crlf);
        conn->send(getUser(user) + "\r\n");
        buf->retrieveUntil(crlf + 2);
        conn->shutdown();
    }
}

int main(int argc, const char *argv[])
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(8888), "Finger");
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
    return 0;
}
