#include <log/callstack.h>
#include <utils/thread.h>

#define LOG_TAG "callstack"

namespace ns
{
    template <typename T, typename U>
    void func(T t, U u)
    {
        Jarvis::CallStack cs;
        cs.update();
        Jarvis::String8 str = cs.toString();
        printf("%s\n\n", str.c_str());
        cs.log("callstack", Jarvis::LogLevel::INFO);
    }
}

template <typename T>
struct Len
{
public:
    void len()
    {
        Jarvis::String8 s;
        ns::func(t, s);
    }
private:
    T t;
};

int thread(void *arg)
{
    Len<int> l;
    l.len();
    return 0;
}

int main()
{
    Len<int> l;
    l.len();

    Jarvis::Thread th("Thread", thread);
    th.run();
    sleep(2);
    return 0;
}