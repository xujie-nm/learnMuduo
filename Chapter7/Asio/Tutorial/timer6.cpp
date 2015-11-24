#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

class Printer : boost::noncopyable{
    public:
        Printer(muduo::net::EventLoop* loop1, muduo::net::EventLoop* loop2)
            : loop1_(loop1),
              loop2_(loop2),
              count_(0){
            loop1_->runAfter(1, boost::bind(&Printer::print1, this));
            loop2_->runAfter(1, boost::bind(&Printer::print2, this));
        }

        ~Printer(){
            // cout is not thread safe
            // std::cout << "Final count is" << count_ << "\n";
            printf("Final count is %d\n", count_);
        }

        void print1(){
            bool shouldQuit = false;
            int count = 0;
            
            // Critical region
            {
                muduo:: MutexLockGuard lock(mutex_);
                if(count_ < 10){
                    count  = count_;
                    ++count;
                }else{
                    shouldQuit = true;
                }
            }

            // out of lock
            if(shouldQuit){
                loop1_->quit();
            }else{
                printf("Timer 1: %d\n", count);
                loop1_->runAfter(1, boost::bind(&Printer::print1, this));
            }
        }

        void print2(){
            bool shouldQuit = false;
            int count = 0;

            {
                muduo::MutexLockGuard lock(mutex_);
                if(count_ < 10){
                    count = count_;
                    ++count_;
                }else{
                    shouldQuit = true;
                }
            }
            
            // out of lock
            if(shouldQuit){
                loop2_->quit();
            }else{
                printf("Timer 2: %d\n", count);
                loop2_->runAfter(1, boost::bind(&Printer::print2, this));
            }
        }

    private:

        muduo::MutexLock mutex_;
        muduo::net::EventLoop* loop1_;
        muduo::net::EventLoop* loop2_;
        int count_;
};

// 利用一个副本缩小临界区
int main(int argc, const char *argv[])
{
    boost::scoped_ptr<Printer> printer;// make sure printer lives longer than loops, to avoid
                                       // race condition of calling print2() on destructed object

    muduo::net::EventLoop loop;
    muduo::net::EventLoopThread loopThread;
    muduo::net::EventLoop* loopInAnotherThread = loopThread.startLoop();
    printer.reset(new Printer(&loop, loopInAnotherThread));
    loop.loop();
    return 0;
}
