#include "../Mutex.h"
#include "../Thread.h"
#include <vector>
#include <stdio.h>

using namespace muduo;

class Foo{
    public:
        void doit()const;
};

MutexLock mutex;
std::vector<Foo> foos;

void post(const Foo&f){
    MutexLockGuard lock(mutex);
    foos.push_back(f);
}

void traverse(){
    MutexLockGuard lock(mutex);
    for(std::vector<Foo>::const_iterator it = foos.begin(); it != foos.end(); ++it){
        it->doit();    
    }
}

void Foo::doit()const{
    Foo f;
    post(f);
}

int main(int argc, const char *argv[])
{
    Foo f;
    post(f);
    traverse();
    return 0;
}
// 这里的mutex如果是不可重入的，那么traverse里加了锁
// 然后调用doit，但是doit里调用了post，post里又要申请
// 加锁，必然会导致死锁。
// 如果这里的mutex是可重入的，那么doit里的加锁是可以
// 成功的，但是post里修改了vector会导致迭代器失效，所
// 以traverse里的for会出问题。
// （C/C++里的mutex是不可重入的）
