#ifndef __NOOBNET_THREAD_
#define __NOOBNET_THREAD_

#include <thread>
#include <pthread.h>
#include <functional>
#include <memory>
#include "mutex.h"

namespace noobnet {

class Thread {
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();

    pid_t getPid() const { return m_pid; }

    void join();
    const std::string& getName() const { return m_name; }

    static Thread* GetThis();
    static const std::string& GetName();
    static void SetName(const std::string& name);
private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator= (const Thread&) = delete;

    static void* run(void* arg);
private:
    pthread_t m_thread = 0;
    pid_t m_pid = -1;
    std::function<void()> m_cb;
    std::string m_name;
    Semophore m_semophore;
};

} // noobnet

#endif // !__NOOBNET_THREAD_