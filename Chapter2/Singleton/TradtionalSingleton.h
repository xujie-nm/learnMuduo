template <typename T>
class Singleton{
    public:
        static T& Instance(){
            if(m_pInstance == NULL){
                Lock lock;
                if(m_pLinstance == NULL){
                    m_pInstance = new T();
                    atexit(Destory);
                }
                return *m_pInstance;
            }
            return *_pInstance;
        }

    protected:
        Singleton(){}
        ~Singleton(){}

    private:
        Singleton(const Singleton &rhs) {}
        Singleton& operator = (const Singleton &rhs) {}

        void Destory(){
            if(m_pInstance != NULL){
                delete m_pInstance;
            }
            m_pInstance = NULL;
        }
            
        static T* volatile m_pInstance;
};

template<typename T>
T* Singleton<T>::m_pInstance = NULL;
