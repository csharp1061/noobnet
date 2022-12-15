#ifndef __NOOBNET_NONCOPYABLE_
#define __NOOBNET_NOCOPYABLE_

namespace noobnet {
/**
 * @brief 禁止复制函数类，继承该函数将禁止派生类的复制行为
*/
class Noncopyable {
public:
    /**
     * @brief 默认构造函数
    */
    Noncopyable() = default;
    /**
     * @brief 默认析构函数
    */
    ~Noncopyable() = default;
    /**
     * @brief 拷贝构造函数（禁止）
    */
    Noncopyable(const Noncopyable&) = delete;
    /**
     * @brief 赋值运算符（禁止）
    */
    Noncopyable& operator=(const Noncopyable&) = delete;
};

} //noobnet

#endif // !__NOOBNET_NONCOPYABLE_
