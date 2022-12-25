#include "fiber.h"
#include "log.h"
#include "config.h"
#include "macro.h"
#include <atomic>

namespace noobnet {

static Logger::ptr g_logger = SYS_LOG_NAME("system");

static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadfiber = nullptr;

static ConfigVar<uint32_t>::ptr g_fiber_stacksize = 
    Config::LookUp<uint32_t>(128*1024, "fiber.stacksize", "fiber stack size");

class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }

    static void DeAlloc(void* vp, size_t size) {
        free(vp);
    }
}

using StackAllocator = MallocStackAllocator;

uint64_t Fiber::GetFiberId() {
    if (t_fiber) {
        return t_fiber->getId();
    }
    return 0;
}

Fiber::Fiber() {
    m_state = EXEC;
    SetThis(this);

    if (getcontext(&m_ctx)) {
        SYS_ASSERT2(false, "getcontext");
    }

    ++s_fiber_count;
    SYS_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_call) 
        :m_id(++s_fiber_id)
        ,m_cb(cb) {
    ++s_fiber_count;
    m_stacksize = stacksize ? stacksize : g_fiber_stacksize->getValue();

    m_stack = StackAllocator::Alloc(m_stacksize);
    if (getcontext(&m_ctx)) {
        SYS_ASSERT2(false, "getcontext");
    }
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    if (!use_call) {
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    } else {
        makekcontext(&m_ctx, &Fiber::CallerMainFunc, 0)
    }

    SYS_LOG_DEBUG(g_logger) << "Fiber::Fiber id= " << m_id;
}

Fiber::~Fiber() {
    --s_fiber_count;
    if (m_stack) {
        SYS_ASSERT(m_state == TERM
                || m_state == EXCEPT
                || m_state == INIT);
        StackAllocator::DeAlloc(m_stack, m_stacksize);
    } else {
        SYS_ASSERT(!m_cb);
        SYS_ASSERT(m_state = EXEC);

        Fiber* cur = t_fiber;
        if (cur == this) {
            SetThis(nullptr);
        }
    }
    SYS_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id
                            << " total=" << s_fiber_count;
}   

//重置协程函数，并且重置状态

} // noobnet