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
            printf("in ~Request()\n");
            sleep(1);//为了容易复现死锁，延时操作
            g_inventory.remove(this);
        }

        void print() const// __attribute__((noinline))
        {
            muduo::MutexLockGuard lock(mutex_);
            printf("Request::print()\n");
        }

    private:
        mutable muduo::MutexLock mutex_;
};

void Inventory::printAll()const{
    muduo::MutexLockGuard lock(mutex_);
    sleep(1);//为了容易复现死锁，延时操作
    for(std::set<Request*>::const_iterator it = requests_.begin(); 
            it != requests_.end(); ++it){
        (*it)->print();   
    }
    printf("Inventory::printAll() unlocked\n");
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
    sleep(1);
    // 这里对～Request进行延迟操作
    // 就会让～Request和print发生死锁
    delete req;
}

int main(int argc, const char *argv[])
{
    muduo::Thread thread(threadFunc);
    thread.start();
    usleep(500 * 1000);//0,5秒,这里依旧不能删去，为了让thread往requests_加入对象
    g_inventory.printAll();
    thread.join();
    return 0;
}
