#include "../Chargen/chargen.h"
#include "../Daytime/daytime.h"
#include "../Discard/discard.h"
#include "../Echo/echo.h"
#include "../Time/time.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

int main(int argc, const char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    // one loop shared by multiple servers
    EventLoop loop;

    ChargenServer chargenServer(&loop, InetAddress(8881));
    chargenServer.start();

    DaytimeServer daytimeServer(&loop, InetAddress(8882));
    daytimeServer.start();

    DiscardServer discardServer(&loop, InetAddress(8883));
    discardServer.start();

    EchoServer echoServer(&loop, InetAddress(8884));
    echoServer.start();

    TimeServer timeServer(&loop, InetAddress(8885));
    timeServer.start();

    loop.loop();
    return 0;
}
