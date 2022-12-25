#ifndef __NOOBNET_FIBER_
#define __NOOBNET_FIBER_

#include <functional>
#include <memory>
#include <ucontext.h>

namespace noobnet {

/**
 * @brief 携程类封装
*/
class Fiber : public std::enable_shared_from_this<Fiber> {
public:
    typedef std::shared_ptr<Fiber> ptr;

    //协程状态
    enum State {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };
    
private:
    /**
     * @brief 无参构造函数
    */
    Fiber();
public:
    /**
     * @brief 有参构造函数
     * @param[in] cb 回调函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否接收主协程的调度
    */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);

    /**
     * @brief 析构函数
    */
    ~Fiber();

    /**
     * @brief 重设协程状态及回调函数
     * @pre getState() 为 INIT TERM EXCEPT
     * @post getState() INIT
    */
    void reset(std::function<void()> cb);

    /**
     * @brief 将当前协程切换至运行状态
     * @pre getState() != EXEC
     * @post  getState = EXEC
    */
    void swapIn();

    /**
     * @brief 将当前协程切换至后台
    */
    void swapOut();

    /**
     * @brief
    */
   void call();

    /**
     * @brief
    */
    void back();

    /**
     * @brief 返回协程id
    */
    uint64_t getId() const { return m_id; }

    /**
     * @brief 获得协程状态
    */
    State getState() const { return m_state; }
public:
    /**
     * @brief 设置当前线程的运行协程
    */
    static void SetThis(Fiber* fiber);

    /**
     * @brief 获取当前线程的运行协程
    */
    static Fiber::ptr GetThis();

    /**
     * @brief 将当前协程切换至后台并将状态设置为ready状态
    */
    static void YieldToReady();

    /**
     * @brief 将当前协程切换至后台并将状态设置为hold状态
    */
    static void YieldToHold();

    /**
     * @brief 获取当前的总协程数
    */
    static uint64_t TotalFibers();

    /**
     * @brief 当前协程的执行函数
     * @post 回到主协程
    */
    static void MainFunc();

    /**
     * @brief 当前协程的执行函数
     * @post 回到调度协程
    */
    static void CallerMainFunc();

    /**
     * @brief 获取当前线程上的协程id
    */
    static uint64_t GetFiberId();
private:
    // 协程id
    uint64_t m_id = 0;
    // 协程栈大小
    uint32_t m_stacksize = 0;
    // 协程状态
    State m_state;
    // 协程上下文
    ucontext_t m_ctx;
    // 指向分配的协程zhan
    void* m_stack = nullptr;
    // 协程执行的回调函数
    std::function<void()> m_cb;
};

} // noobnet

#endif // !__NOOBNET_FIBER_