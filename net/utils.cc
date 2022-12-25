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

static std::string demangle(const char* str) {
    size_t size = 0;
    int status = 0;
    std::string rt;
    rt.resize(256);
    if (1 == sscanf(str, "%*[^(]%*[^_]%255[^)+]", &rt[0])) {
        char* v = abi::__cxa_demangle(&rt[0], nullptr, &size, &status);
        if (v) {
            std::string result(v);
            free(v);
            return result;
        }
    }
    if (1 == sscanf(str, "%255s", &rt[0])) {
        return rt;
    }
    return str;
}

void Backtrace(std::vector<std::string>& vec, int size, int skip) {
    void** array = (void**)malloc((sizeof(void*) * size));
    size_t s = ::backtrace(array, size);

    char** strings = backtrace_symbols(array, s);
    if (strings == nullptr) {
        SYS_LOG_ERROR(g_logger) << "backtrace symbols error";
        return;
    }

    for (size_t i = skip; i < s; ++i) {
        vec.push_back(demangle(strings[i]));
    }

    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix) {
    std::vector<std::string> vec;
    Backtrace(vec, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        ss << prefix << vec[i] << std::endl;
    }
    return ss.str();
}

} // noobnet