#ifndef __NOOBNET_SEMAPHORE_
#define __NOOBNET_SEMAPHORE_

#include "noncopyable.h"
#include <thread>
#include <memory>
#include <functional>
#include <list>
#include <stdint.h>
#include <semaphore.h>

namespace noobnet {

/**
 * @brief 信号量的实现
*/
class Semophore : public Noncopyable {
public:
    /**
     * @brief 构造函数
     * @param[in] count 信号量值的大小
    */
    Semophore(uint32_t count = 0);

    /**
     * @brief 析构函数
     *      不要在析构函数中抛出异常，可能会导致对象并未完全回收
    */
    ~Semophore();

    /**
     * @brief 获取信号量
    */
    void wait();

    /**
     * @brief 释放信号量
    */
    void notify();
private:
    sem_t m_semophore;
};

/**
 * @brief 局部锁
*/
template<class T>
struct ScopeLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 互斥锁Mutex
    */
    ScopeLockImpl(T& mutex) : m_mutex(mutex) {
        m_mutex.lock();
        m_islocked = true;
    }

    /**
     * @brief 析构函数
    */
    ~ScopeLockImpl() {
        unlock();
    }

    /**
     * @brief 上锁
    */
    void lock() {
        if (!m_islocked) {
            m_mutex.lock();
            m_islocked = true;
        }
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        if (m_islocked) {
            m_mutex.unlock();
            m_islocked = false;
        }
    }
private:
    T& m_mutex;
    bool m_islocked;
};

/**
 * @brief 读局部锁
*/
template<class T>
struct ReadScopeLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex RWMutex
    */
    ReadScopeLockImpl(T& mutex) : m_mutex(mutex) {
        m_mutex.rdlock();
        m_islocked = true;
    }

    /**
     * @brief 析构函数
    */
    ~ReadScopeLockImpl() {
        unlock();
    }

    /**
     * @brief 上锁
    */
    void lock() {
        if (!m_islocked) {
            m_mutex.rdlock();
            m_islocked = true;
        }
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        if (m_islocked) {
            m_mutex.unlock();
            m_islocked = false;
        }
    }
private:
    T& m_mutex;
    bool m_islocked;
};

/**
 * @brief 写局部锁
*/
template<class T>
struct WriteScopeLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex RWMutex
    */
    WriteScopeLockImpl(T& mutex) : m_mutex(mutex) {
        m_mutex.wrlock();
        m_islocked = true;
    }

    /**
     * @brief 析构函数
    */
    ~WriteScopeLockImpl() {
        unlock();
    }

    /**
     * @brief 上锁
    */
    void lock() {
        if (!m_islocked) {
            m_mutex.wrlock();
            m_islocked = true;
        }
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        if (m_islocked) {
            m_mutex.unlock();
            m_islocked = false;
        }
    }
private:
    T& m_mutex;
    bool m_islocked;
};
/**
 * @brief 互斥锁
*/
class Mutex : Noncopyable {
public:
    typedef ScopeLockImpl<Mutex> Lock;

    /**
     * @brief 构造函数
    */
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    /**
     * @brief 析构函数
    */
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    /**
     * @brief 上锁
    */
    void lock() {
        pthread_mutex_lock(&m_mutex); //?
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

/**
 * @brief 空锁，用于调试
*/
class NullMutex : Noncopyable {
public:
    typedef ScopeLockImpl<NullMutex> Lock;

    /**
     * @brief 构造函数
    */
    NullMutex() {}

    /**
     * @brief 析构函数
    */
    ~NullMutex() {}

    /**
     * @brief 上锁
    */
    void lock() {}

    /**
     * @brief 解锁
    */
    void unlock() {}
};

/**
 * @brief 读写锁
*/
class RWMutex : Noncopyable {
public:
    // 局部读锁
    typedef ReadScopeLockImpl<RWMutex> ReadLock;
    // 局部写锁
    typedef WriteScopeLockImpl<RWMutex> WriteLock;

    /**
     * @brief 构造函数
    */
    RWMutex() {
        pthread_rwlock_init(&m_mutex, nullptr);
    }

    /**
     * @brief 析构函数
    */
    ~RWMutex() {
        pthread_rwlock_destroy(&m_mutex);
    }

    /**
     * @brief 读上锁
    */
    void rdlock() {
        pthread_rwlock_rdlock(&m_mutex);
    }

    /**
     * @brief 写上锁
    */
    void wrlock() {
        pthread_rwlock_wrlock(&m_mutex);
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        pthread_rwlock_unlock(&m_mutex);
    }
private:
    pthread_rwlock_t m_mutex;
};

/**
 * @brief 读写空锁（用于调试）
*/
class NullRWMutex : public Noncopyable {
public:
    // 局部读锁
    typedef ReadScopeLockImpl<RWMutex> ReadLock;
    // 局部写锁
    typedef WriteScopeLockImpl<RWMutex> WriteLock;

    /**
     * @brief 构造函数
    */
    NullRWMutex() {}

    /**
     * @brief 析构函数
    */
    ~NullRWMutex() {}

    /**
     * @brief 上锁
    */
    void lock() {}

    /**
     * @brief 解锁
    */
    void unlock() {}
};

/**
 * @brief 自旋锁
*/
class SpinLock : public Noncopyable {
public:
    // 局部自旋锁
    typedef ScopeLockImpl<SpinLock> Lock;

    /**
     * @brief 构造函数
    */
    SpinLock() {
        pthread_spin_init(&m_mutex, 0);
    }

    /**
     * @brief 析构函数
    */
    ~SpinLock() {
        pthread_spin_destroy(&m_mutex);
    }

    /**
     * @brief 上锁
    */
    void lock() {
        pthread_spin_lock(&m_mutex);
    }

    /**
     * @brief 解锁
    */
    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;
};

}  // noobnet

#endif // !__NOOBNET_SEMAPHORE_
