#include <mutex>

using namespace std;

template<typename T>
class Share_Ptr{
public:
    // 构造函数
    Share_Ptr(T* ptr = nullptr)
        :_ptr(ptr),
        _refCount(new int(1)),
        _mutex(new mutex())
    {}
    // 析构函数
    ~Share_Ptr(){
        Release();
    }
    // 拷贝构造函数
    Share_Ptr(const Share_Ptr<T>& sp)
        :_ptr(sp._ptr),
        _refCount(sp._refCount),
        _mutex(sp._mutex)
    {
        AddRefCount();
    }
    // 赋值重载
    Share_Ptr<T>& operator=(const Share_Ptr<T>& sp){
        // 需要对原来的指针引用减一，并对新的指针引用加一
        if(_ptr != sp._ptr){
            Release();
            _ptr = sp._ptr;
            _refCount = sp._refCount;
            _mutex = sp._mutex;
            AddRefCount();
        }
        return *this;
    }

    // 取值运算符重载
    T& operator*(){
        return *_ptr;
    }

    // ->指针运算符重载
    T* operator->(){
        return _ptr;
    }

    T* get(){
        return _ptr;
    }
    int useCount(){
        return *_refCount;
    }
    // 引用计数加1，要保证线程安全
    void AddRefCount(){
        _mutex->lock();
        (*_refCount)++;
        _mutex->unlock();
    }
private:
    void Release(){
        //先将引用计数减1，判断是否为0，如果为0则释放指针指向的空间
        // 保证线程安全
        bool deleteFlag = false;
        _mutex->lock();
        if(--(*_refCount) == 0){
            delete _ptr;
            delete _refCount;
            deleteFlag = true;
        }
        _mutex->unlock();
        if(deleteFlag){
            delete _mutex;
        }
    }

private:
    T* _ptr; // 原生指针
    int *_refCount; // 引用计数
    mutex *_mutex; // 互斥锁保护引用计数
};
