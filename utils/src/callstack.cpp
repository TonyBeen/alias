/*************************************************************************
    > File Name: callstack.cpp
    > Author: hsz
    > Mail:
    > Created Time: Tue 27 Jul 2021 06:02:27 PM CST
 ************************************************************************/

#define UNW_LOCAL_ONLY
#include "callstack.h"
#include <cxxabi.h>
#include <libunwind.h>
#include <stdlib.h>

namespace Alias {
CallStack::CallStack()
{

}

CallStack::CallStack(const char* logtag, int32_t ignoreDepth)
{
    this->update(ignoreDepth + 1);
    this->log(logtag);
}

CallStack::~CallStack()
{
    
}

void CallStack::update(int32_t ignoreDepth)
{
    mSkip = ignoreDepth;
    unw_cursor_t cursor;
    unw_context_t context;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    while (unw_step(&cursor) > 0)
    {
        unw_word_t offset, funcPointer;
        unw_get_reg(&cursor, UNW_REG_IP, &funcPointer);
        if (funcPointer == 0) {
            break;
        }

        char sym[256];
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
            char *nameptr = sym;
            int status = -1;
            char *demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if (status == 0) {
                nameptr = demangled;
            }
            mStackFrame.push_back(std::move(String8::format("-0x%016lx: (%s + 0x%lx)", funcPointer, nameptr, offset)));
            std::free(demangled);
        } else {
            mStackFrame.push_back(std::move(String8::format("(unable to obtain symbol name for this frame)")));
        }
    }
}

void CallStack::log(const char* logtag, LogLevel::Level level) const
{
    for (size_t i = 0; i < mStackFrame.size() - mSkip; ++i) {
        log_write(level, logtag, "%s\n", mStackFrame[i].c_str());
    }
}

String8 CallStack::toString() const
{
    String8 str;
    for (size_t i = 0; i < mStackFrame.size(); ++i) {
        str += mStackFrame[i];
        str += "\n";
    }
    return std::move(str);
}

} // namespace Alias