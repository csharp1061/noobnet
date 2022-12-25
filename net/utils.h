#ifndef __NOOBNET_UTILS_
#define __NOOBNET_UTILS_

#include <cxxabi.h>
#include <pthread.h>
#include <execinfo.h>
#include <vector>
#include <string>

namespace noobnet {

template<class T>
const char* TypeToName() {
  static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
  return s_name;
}

pid_t GetThreadId();

void Backtrace(std::vector<std::string>& vec, int size = 64, int skip = 1);

std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");
} // noobnet
#endif // !__NOOBNET_UTILS_