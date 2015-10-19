#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "../Mutex.h"

using std::string;

class CustomerData : boost::noncopyable{
    public:
        CustomerData() : data_(new Map){
        }

        int query(const string& customer, const string& stock)const;

    private:
        typedef std::pair<string, int> Entry;
        typedef std::vector<Entry> EntryList;
        typedef std::map<string, EntryList> Map;
        typedef boost::shared_ptr<Map> MapPtr;
        void update(const string& customer, const EntryList& entries);
        void update(const string& message);

        static int findEntry(const EntryList& entries, const string& stock);
        static MapPtr parseData(const string& message);
        // 具体处理逻辑

        MapPtr getData() const{
            muduo::MutexLockGuard lock(mutex_);
            return data_;
        }

        mutable muduo::MutexLock mutex_;
        MapPtr data_;
};

int CustomerData::query(const string& customer, const string& stock) const{
    MapPtr data = getData();
    // getdata里有锁，在拿到数据之后就不需要锁了

    Map::const_iterator entries = data->find(customer);
    if(entries != data->end())
        return findEntry(entries->second, stock);
    else
        return -1;
}

void CustomerData::update(const string& customer, const EntryList& entries){
    muduo::MutexLockGuard lock(mutex_);
    if(!data_.unique()){
        MapPtr newData(new Map(*data_));
        // 在这里打印日志，然后统计日志来判断 Worst case 发生的次数
        data_.swap(newData);
    }
    assert(data_.unique());
    (*data_)[customer] = entries;
}

void CustomerData::update(const string& message){
    // 解析新数据，在临界区之外
    MapPtr newData = parseData(message);
    if(newData){
        muduo::MutexLockGuard lock(mutex_);
        data_.swap(newData);
    }
    // 旧数据的析构也在临界区之外没进一步缩短了临界区
}

int main(int argc, const char *argv[])
{
    CustomerData data;
    return 0;
}
