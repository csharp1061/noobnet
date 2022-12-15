#include "utils.h"
#include "log.h"
#include <unistd.h>
#include <sys/syscall.h>

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE

#endif // !_GNU_SOURCE


namespace noobnet {

static noobnet::Logger::ptr g_logger = SYS_LOG_NAME("system");

pid_t GetThreadId() {
    return syscall(SYS_gettid);
}

} // noobnet