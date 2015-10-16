#include "../Mutex.h"

class Request{
    public:
        void process(){
            muduo::MutexLockGuard lock(mutex_);
            print();
        }

        void print() const{
            muduo::MutexLockGuard lock(mutex_);
        }

    private:
        mutable muduo::MutexLock mutex_;
};

int main(int argc, const char *argv[])
{
    Request req;
    req.process();
    return 0;
}
