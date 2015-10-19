#include "../Mutex.h"
#include "../Thread.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <stdio.h>

using namespace muduo;

class Foo{
    public:
        void doit() const;
};

typedef std::vector<Foo> FooList;
typedef boost::shared_ptr<FooList> FooListPtr;
FooListPtr g_foos;
MutexLock mutex;

void post(const Foo& f){
    printf("post\n");
    MutexLockGuard lock(mutex);
    if(!g_foos.unique()){
        g_foos.reset(new FooList(*g_foos));
        // 用原来的复制
        // 这样的话原来的 g_foosde 迭代器不会失效
        printf("copy the whole list\n");
        // 引用数不是1，所以在副本上修改
    }
    assert(g_foos.unique());
    g_foos->push_back(f);
}

/*
void post(const Foo& f){
    printf("post\n");
    {
        MutexLockGuard lock(mutex);
        if(!g_foos.unique()){
            g_foos.reset(new FooList(*g_foos));
            // 用原来的复制
            // 这样的话原来的 g_foosde 迭代器不会失效
            // 引用数不是1，所以在副本上修改
        }
        assert(g_foos.unique());
        g_foos->push_back(f);
    }
    printf("copy the whole list\n");
    // 将这句话移出临界区
}
*/

/*错误一: 直接修改，迭代器失效
void post(const Foo& f){
    MutexLockGuard lock(mutex);
    g_foos->push_back(f);
}
*/

/*错误二: 试图缩小临界区， 把copying移出临界区
 * 修改要放到临界区内,缺点是除了每次复制全部以外，
 * 放到临界区外的复制如果又多个post一起运行就会出现其他问题
void post(const Foo& f){
    FooListPtr newFoos(new FooList(*g_foos));
    newFoos->push_back(f);
    MutexLockGuard lock(mutex);
    g_foos = newFoos;
}
*/

/*错误三: 把临界区拆分为两个小的，copying依旧会放到临界区
void post(const Foo& f){
    FooListPtr oldFoos;
    {
        MutexLockGuard lock(mutex);
        oldFoos = g_foos;
    }
    FooListPtr newFoos(new FooList(*oldFoos));
    newFoos->push_back(f);
    MutexLockGuard lock(mutex);
    g_foos = newFoos;
}
*/


void traverse(){
    FooListPtr foos;
    {
        MutexLockGuard lock(mutex);
        foos = g_foos;
        assert(!g_foos.unique());
    }

    // assert(!foos.unique()); this may not hold

    for(std::vector<Foo>::const_iterator it = foos->begin(); 
            it != foos->end(); ++it){
        it->doit();
    }
}

void Foo::doit() const{
    Foo f;
    post(f);
}

int main(int argc, const char *argv[])
{
    g_foos.reset(new FooList);
    Foo f;
    post(f);
    traverse();
    return 0;
}
