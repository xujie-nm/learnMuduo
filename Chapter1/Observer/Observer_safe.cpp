#include <algorithm>
#include <vector>
#include <stdio.h>
#include <muduo/base/Mutex.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Observable;

class Observer : public boost::enable_shared_from_this<Observer>{
    public:
        virtual ~Observer();
        virtual void update()=0;

        void observe(Observable *s);

    protected:
        Observable *subject_;
};

class Observable{
    public:
        void register_(boost::weak_ptr<Observer> x);//参数类型可用const weak_ptr<Observer>&
        //void unregister(boost::weak_ptr<Observer> x);不需要它

        void notifyObservers(){
            muduo::MutexLockGuard lock(mutex_);
            Iterator it = observers_.begin();
            while(it != observers_.end()){
                boost::shared_ptr<Observer> obj(it->lock());//尝试提升，这一步是线程安全的
                if(obj){
                    //提升成功，现在的引用计数几乎总是大于等于二的
                    //猜测本身一个，lock（）一个。
                    obj->update();
                    ++it;
                }else{
                    printf("notifyObservers() erase\n");
                    //对象销毁拿掉
                    it = observers_.erase(it);
                }
            }
        }

    private:
        mutable muduo::MutexLock mutex_;
        std::vector<boost::weak_ptr<Observer> > observers_;
        typedef std::vector<boost::weak_ptr<Observer> >::iterator Iterator;
};

Observer::~Observer(){
    // subject_->unregister(this);
}

void Observer::observe(Observable *s){
    s->register_(shared_from_this());
    subject_ = s;
}

void Observable::register_(boost::weak_ptr<Observer> x){
    observers_.push_back(x);
}

class Foo : public Observer{
    virtual void update(){
        printf("Foo::update() %p\n", this);
    }
};

int main(int argc, const char *argv[])
{
    Observable subject;
    {
        boost::shared_ptr<Foo> p(new Foo);
        p->observe(&subject);
        subject.notifyObservers();
    }
    subject.notifyObservers();
    return 0;
}
