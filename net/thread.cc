#include "thread.h"
#include "utils.h"
#include "log.h"


namespace noobnet {

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOWN";

static noobnet::Logger::ptr g_logger = SYS_LOG_NAME("system");

Thread* Thread::GetThis() {
    return t_thread;
}

const std::string& Thread::GetName() {
    return t_thread_name;
}

void Thread::SetName(const std::string& name) {
    if (name.empty()) {
        return;
    }
    if (t_thread) {
        t_thread->m_name = name;
    }
    t_thread_name = name;
}

Thread::Thread(std::function<void()> cb, const std::string& name) 
               : m_cb(cb)
               , m_name(name) {
    if (name.empty()) {
        m_name = "UNKOWN";
    }
    int ret = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if (ret) {
        SYS_LOG_ERROR(g_logger) << "Thread create fail--ret= " <<
            ret << "--name= " << name;
        throw std::logic_error("Thread create fail");
    }
    m_semophore.wait();
}

Thread::~Thread() {
    if (m_thread) {
        pthread_detach(m_thread);
    }
}

void Thread::join() {
    if (m_thread) {
        int ret = pthread_join(m_thread, nullptr);
        if (ret) {
            SYS_LOG_ERROR(g_logger) << "Thread join fail--ret= " <<
                ret << "--name= " << m_name;
            throw std::logic_error("Thread join fail");
        }
        m_thread = 0;
    }
}

void* Thread::run(void* arg) {
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_pid = noobnet::GetThreadId();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    cb.swap(thread->m_cb);

    thread->m_semophore.notify();

    cb();
    return 0;
}

} // noobnet


