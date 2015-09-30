#include <muduo/base/Mutex.h>
#include <muduo/base/CurrentThread.h>
#include <boost/noncopyable.hpp>
//#include <pthread.h>
using namespace muduo;

//a thread-safe counter
class Counter: boost::noncopyable{
    public:
        Counter(): value_(0){}

        Counter& operator=(const Counter& rhs);

        int value()const;
        int getAndIncrease();

        friend void swap(Counter &a, Counter &b);
    
    private:
        mutable MutexLock mutex_;
        int value_;
};

int Counter::value()const{
    MutexLockGuard lock(mutex_);
    return value_;
}

int Counter::getAndIncrease(){
    MutexLockGuard lock(mutex_);
    int ret = value_++;
    return ret;
}

Counter& Counter::operator=(const Counter& rhs){
    if(this == &rhs)
        return *this;//potential dead lock

    MutexLockGuard myLock(mutex_);
    MutexLockGuard itsLock(rhs.mutex_);
    value_ = rhs.value_;
    return *this;
}

void swap(Counter &a, Counter &b){
    MutexLockGuard aLock(a.mutex_);//potential dead lock
    MutexLockGuard bLock(b.mutex_);
    int value = a.value_;
    a.value_ = b.value_;
    b.value_ = value;
}
int main(){
    Counter c;
    c.getAndIncrease();
}
