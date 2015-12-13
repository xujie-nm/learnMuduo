#ifndef MUDUO_NET_CALLBACKS_H
#define MUDUO_NET_CALLBACKS_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "datetime/Timestamp.h"

namespace muduo{
    
    // All client visible callbacks go here.

    typedef boost::function<void()> TimerCallback;

}

#endif // MUDUO_NET_CALLBACKS_H
