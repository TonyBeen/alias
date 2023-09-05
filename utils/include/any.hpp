/*************************************************************************
    > File Name: any.hpp
    > Author: hsz
    > Brief: copy from https://github.com/Qihoo360/simcc/blob/master/simcc/any.h
    > Created Time: Fri 09 Dec 2022 03:11:20 PM CST
 ************************************************************************/

#ifndef __UTILS_ANY_HPP__
#define __UTILS_ANY_HPP__

#include <typeinfo>
#include <utility>
#include <memory>
#include <assert.h>

namespace eular {
// NOTE: class Any need the copy structure
class Any {
public:
    Any() : m_placeHolder(nullptr) {}
    ~Any() {}

    template<typename ValueType>
    explicit Any(const ValueType& value)
        : m_placeHolder(new Holder<ValueType>(value)) {}

    Any(const Any& rhs)
        : m_placeHolder(rhs.m_placeHolder) {}

public:
    void swap(Any& rhs) {
        if (this != &rhs) {
            std::swap(m_placeHolder, rhs.m_placeHolder);
        }
    }

    template<typename ValueType>
    Any& operator=(const ValueType& rhs) {
        Any(rhs).swap(*this);
        return *this;
    }

    Any& operator=(const Any& rhs) {
        Any(rhs).swap(*this);
        return *this;
    }

    bool empty() const {
        return !m_placeHolder;
    }

    const std::type_info& type() const {
        return m_placeHolder ? m_placeHolder->getType() : typeid(void);
    }

    template<typename ValueType>
    ValueType *to() {
        if (m_placeHolder != nullptr && m_placeHolder->getType() == typeid(ValueType)) {
            return &(static_cast<Any::Holder<ValueType>*>(m_placeHolder.get())->m_valueHolder);
        }

        return nullptr;
    }

    template<typename ValueType>
    const ValueType *to() const {
        if (m_placeHolder != nullptr && m_placeHolder->getType() == typeid(ValueType)) {
            return &(static_cast<Any::Holder<ValueType> *>(m_placeHolder.get())->m_valueHolder);
        }

        return nullptr;
    }

protected:
    class PlaceHolder {
    public:
        virtual ~PlaceHolder() {}
    public:
        virtual const std::type_info& getType() const = 0;
        virtual PlaceHolder* clone() const = 0;
    };

    template<typename ValueType>
    class Holder : public PlaceHolder {
    public:
        Holder(const ValueType& value)
            : m_valueHolder(value) {}

        virtual const std::type_info& getType() const {
            return typeid(ValueType);
        }

        virtual PlaceHolder* clone() const {
            return new Holder(m_valueHolder);
        }

        ValueType m_valueHolder;
    };

protected:
    std::shared_ptr<PlaceHolder> m_placeHolder;

    template<typename ValueType>
    friend ValueType* any_cast(Any*);
};

template<typename ValueType>
ValueType* any_cast(Any* any) {
    if (any && any->type() == typeid(ValueType)) {
        return &(static_cast<Any::Holder<ValueType>*>(any->m_placeHolder)->m_valueHolder);
    }

    return nullptr;
}

template<typename ValueType>
const ValueType *any_cast(const Any* any) {
    return any_cast<ValueType>(const_cast<Any *>(any));
}

template<typename ValueType>
ValueType any_cast(const Any& any) {
    const ValueType *result = any_cast<ValueType>(&any);
    assert(result);

    if (!result) {
        return ValueType();
    }

    return *result;
}

} // namespace eular

#endif // __UTILS_ANY_HPP__