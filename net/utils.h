#ifndef __NOOBNET_UTILS_
#define __NOOBNET_UTILS_

#include <cxxabi.h>
#include <pthread.h>

namespace {

template<class T>
const char* TypeToName() {
  static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
  return s_name;
}

pid_t GetThreadId();

} // noobnet
#endif // !__NOOBNET_UTILS_