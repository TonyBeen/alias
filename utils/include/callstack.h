/*************************************************************************
    > File Name: callstaack.h
    > Author: hsz
    > Mail:
    > Created Time: Tue 27 Jul 2021 06:02:23 PM CST
 ************************************************************************/

#ifndef __ALIAS_CALLSTACK_H__
#define __ALIAS_CALLSTACK_H__

#include "string8.h"
#include <log/log.h>
#include <stdio.h>
#include <vector>

namespace Alias {

class CallStack {
public:
    CallStack();
    CallStack(const char* logtag, int32_t ignoreDepth = 1);
    ~CallStack();

    void clear() { mStackFrame.clear(); }

    // Immediately collect the stack traces for the specified thread.
    // The default is to dump the stack of the current call.
    void update(int32_t ignoreDepth = 2);

    void log(const char* logtag,
             LogLeval::Leval leval = LogLeval::DEBUG) const;

    // Return a string (possibly very long) containing the complete stack trace.
    String8 toString() const;

    // Get the count of stack frames that are in this call stack.
    size_t size() const { return mStackFrame.size(); }

private:
    std::vector<String8>    mStackFrame;
    uint32_t                mSkip;
};
} // namespace Alias

#endif // __ALIAS_CALLSTACK_H__