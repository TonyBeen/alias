#include <log/callstack.h>
#include <utils/thread.h>

#define LOG_TAG "callstack"

namespace ns
{
    template <typename T, typename U>
    void func(T t, U u)
    {
        eular::CallStack cs;
        cs.update();
        eular::String8 str = cs.toString();
        printf("%s\n\n", str.c_str());
        cs.log("callstack", eular::LogLevel::INFO);
    }
}

template <typename T>
struct Len
{
public:
    void len()
    {
        eular::String8 s;
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

    eular::Thread th("Thread", thread);
    th.run();
    sleep(2);
    return 0;
}