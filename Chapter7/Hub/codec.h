#ifndef LEARNMUDUO_CHPATER7_HUB_CODEC_H
#define LEARNMUDUO_CHPATER7_HUB_CODEC_H

// internal header file

#include <muduo/base/Types.h>
#include <muduo/net/Buffer.h>

#include <boost/noncopyable.hpp>

namespace pubsub{
    using muduo::string;

    enum ParseResult{
        kError,
        kSuccess,
        kContinue,
    };

    ParseResult parseMessage(muduo::net::Buffer* buf,
                             string* cmd,
                             string* topic,
                             string* content);
}

#endif
