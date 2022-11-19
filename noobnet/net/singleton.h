#ifndef _SINGLETON_
#define _SINGLETON_

#include <memory>

namespace noobnet {

//返回单例对象
template<class T, class Z = void, int N = 0>
class SingleTon {
public:
  static T* getInstance() {
    static T v;
    return &v;
  }
};

//返回单例智能指针
template<class T, class Z = void, int N = 0>
class SingleTonPtr {
public:
  static std::shared_ptr<T> getInstance() {
    static std::shared_ptr<T> v(new T);
    return v;
  }
};

} //noobnet
#endif // !_SINGLETON_
