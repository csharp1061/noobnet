#ifndef __NOOBNET_MACRO_
#define __NOOBNET_MACRO_

#include "log.h"
#include "utils.h"
#include <assert.h>

#define SYS_ASSERT(x) \
    if (!(x)) { \
        SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << noobnet::BacktraceToString(100, 2, "   "); \
        assert(x); \
    }

#define SYS_ASSERT2(x, w) \
    if (!(x)) { \
        SYS_LOG_ERROR(SYS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << noobnet::BacktraceToString(100, 2, "   "); \
        assert(x);
    }

#endif // !__NOOBNET_MACRO_