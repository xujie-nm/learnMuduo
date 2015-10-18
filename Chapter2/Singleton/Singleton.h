#ifdef SINGLETON_H
#define SINGLETON_H

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

template<typename T>
class Singleton : boost::noncopyable{
    public:
        static T& instance(){
            pthread_once(&ponce_, &Singleton::init);
            assert(value_ != NULL);
            return *value;
        }

    private:
        Singleton();
        ~Singleton();

        static void init(){
            value_ = new T();
            if(!detail::has_no_destory<T>::value){
                ::atexit(destory);
            }
        }

        static void destory(){
            typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
            T_must_be_complete_type dummy; (void) dummy;

            delete value_;
            value_ = NULL;
        }

    private:
        static pthread_once_t ponce_;
        static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

#endif
