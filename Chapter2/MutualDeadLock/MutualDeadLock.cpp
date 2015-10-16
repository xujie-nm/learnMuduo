#include "../Mutex.h"
#include "../Thread.h"
#include <set>
#include <stdio.h>

class Request;

class Inventory{
    public:
        void add(Request* req){
            muduo::MutexLockGuard lock(mutex_);
            requests_.insert(req);
        }

        void remove(Request* req)//__attrbute__((noinline))
        {
            muduo::MutexLockGuard lock(mutex_);
            requests_.erase(req);
        }

        void printAll()const;

    private:
        mutable muduo::MutexLock mutex_;
        std::set<Request*> requests_;
};

Inventory g_inventory;

class Request{
    public:
        void process() // __attribute__((noinline))
        {
            muduo::MutexLockGuard lock(mutex_);
            g_inventory.add(this);
        }

        ~Request() __attribute__((noinline)){
            muduo::MutexLockGuard lock(mutex_);
            sleep(1); //延时，复现死锁
            g_inventory.remove(this);
        }

        void print() const// __attribute__((noinline))
        {
            muduo::MutexLockGuard lock(mutex_);
        }

    private:
        mutable muduo::MutexLock mutex_;
};

void Inventory::printAll()const{
    muduo::MutexLockGuard lock(mutex_);
    sleep(1);// 延时，复现死锁
    for(std::set<Request*>::const_iterator it = requests_.begin(); 
            it != requests_.end(); ++it){
        (*it)->print();   
    }
    printf("Inventory::printAll() unlocked\n");
    // 这里发生死锁的原因是：
    // 是req的process和print发生死锁
}

/*
 void Inventory::printAll() const{
    std::set<Request*> requests;
    {
        muduo::MutexLockGuard lock(mutex_);
        requests = requests_;
    }
    sleep(1);
    for(std::set<Request*>::iterator it = requests.begin(); 
            it != requests.end(); ++it){
        (*it)->print();   
    }
    // 而这里不会发生死锁，
    // 因为重新申请了一个对象后，不是一个锁
 }
 
*/
void threadFunc(){
    Request* req = new Request;
    req->process();
    delete req;
}

int main(int argc, const char *argv[])
{
    muduo::Thread thread(threadFunc);
    thread.start();
    usleep(500 * 1000);//延时0.5秒
    g_inventory.printAll();
    thread.join();
    return 0;
}
