#include "./net/mutex.h"
#include "./net/thread.h"
#include "./net/log.h"
#include <unistd.h>

noobnet::Logger::ptr g_logger = SYS_LOG_ROOT();
int count = 0;
noobnet::RWMutex s_mutex;

void func1() {
    SYS_LOG_INFO(g_logger) << "name: " << noobnet::Thread::GetName()
                           << "this.name: " << noobnet::Thread::GetThis()->getName()
                           << "id: " << noobnet::GetThreadId()
                           << "this.id: " << noobnet::Thread::GetThis()->getPid();
    for (int i = 0; i < 100000; ++i) {
        noobnet::RWMutex::WriteLock lock(s_mutex);
        ++count;
    }
}

void func2() {
}

int main(int argc, char const *argv[])
{
    SYS_LOG_INFO(g_logger) << "Thread test begin" << std::endl;
    std::vector<noobnet::Thread::ptr> thrs;
    for (int i = 0; i < 5; ++i) {
        noobnet::Thread::ptr thr(new noobnet::Thread(&func1, "name_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for (int i = 0; i < 5; ++i) {
        thrs[i]->join();
    }
    SYS_LOG_INFO(g_logger) << "Thread test end" << std::endl;
    SYS_LOG_INFO(g_logger) << "count= " << count;
    return 0;
}
