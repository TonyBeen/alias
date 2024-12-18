/*************************************************************************
    > File Name: thread_local.cpp
    > Author: hsz
    > Brief:
    > Created Time: 2024年12月18日 星期三 10时58分48秒
 ************************************************************************/

#include "utils/thread_local.h"
#include "utils/sysdef.h"
#include "utils/mutex.h"

#if defined(OS_LINUX)
#include <pthread.h>
#endif

namespace eular {

static thread_local std::shared_ptr<ThreadLocalStorage> g_tls = nullptr;

class ThreadLocal
{
public:
    ThreadLocal()
    {
        if (pthread_key_create(&m_key, nullptr)) {
            throw std::runtime_error("pthread_key_create error.");
        }    
    }

    ~ThreadLocal()
    {
        pthread_key_delete(m_key);
    }

    void create()
    {
        uint64_t tid = pthread_self();
        AutoLock<Mutex> lock(m_mtx);
        auto it = m_tlsMap.find(tid);
        if (it == m_tlsMap.end()) {
            m_tlsMap[tid] = std::make_shared<ThreadLocalStorage>();
            pthread_setspecific(m_key, m_tlsMap.at(tid).get());
        }
    }

    void remove()
    {
        uint64_t tid = pthread_self();
        AutoLock<Mutex> lock(m_mtx);
        auto it = m_tlsMap.find(tid);
        if (it != m_tlsMap.end()) {
            pthread_setspecific(m_key, nullptr);
            m_tlsMap.erase(it);
        }
    }

    ThreadLocalStorage *get()
    {
        return reinterpret_cast<ThreadLocalStorage *>(pthread_getspecific(m_key));
    }

    Mutex           m_mtx;
    pthread_key_t   m_key;
    std::map<uint64_t, std::shared_ptr<ThreadLocalStorage>>     m_tlsMap;
} g_threadLocal;

ThreadLocalStorage::ThreadLocalStorage()
{
}

ThreadLocalStorage::~ThreadLocalStorage()
{
}

ThreadLocalStorage *ThreadLocalStorage::Current()
{
    if (g_tls == nullptr) {
        g_tls = std::make_shared<ThreadLocalStorage>();
    }

    return g_tls.get();
}
} // namespace eular
