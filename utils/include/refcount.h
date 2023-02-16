/*************************************************************************
    > File Name: refcount.h
    > Author: hsz
    > Brief:
    > Created Time: Thu 24 Nov 2022 04:48:18 PM CST
 ************************************************************************/

#ifndef __UTILS_REF_COUNT_H__
#define __UTILS_REF_COUNT_H__

#include <atomic>

namespace eular {
class RefCount {
public:
    RefCount() {}
    RefCount(uint32_t init) : atomic_ref_count(init) {}
    ~RefCount() { }

    RefCount(const RefCount &other) = delete;
    RefCount &operator=(const RefCount &other) = delete;

    inline void ref() {
        ++atomic_ref_count;
    }

    inline uint32_t deref() {
        if (atomic_ref_count.load() == 0) {
            return 0;
        }
        --atomic_ref_count;
        return atomic_ref_count.load();
    }

    inline uint32_t load() const {
        return atomic_ref_count;
    }

    std::atomic<uint32_t> atomic_ref_count{0};
};

} // namespace eular

#endif // __UTILS_REF_COUNT_H__
