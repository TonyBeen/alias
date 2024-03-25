/*************************************************************************
    > File Name: bimap.h
    > Author: hsz
    > Brief:
    > Created Time: 2024年03月25日 星期一 09时35分36秒
 ************************************************************************/

#ifndef __EULAR_UTILS_BIMAP_H__
#define __EULAR_UTILS_BIMAP_H__

#include <initializer_list>
#include <unordered_map>

namespace eular {

template <typename K, typename V, typename HashK = std::hash<K>, typename HashV = std::hash<V>>
class BiMap
{
public:
    using iterator = std::unordered_map<K, V>::iterator;
    using const_iterator = std::unordered_map<K, V>::const_iterator;

    BiMap() = default;
    BiMap(const std::initializer_list<std::pair<K, V>> &list);
    ~BiMap();

    void insert(const K& key, const V& value);
    void insert(const V& value, const K& key);

    void erase(const K& key);
    void erase(const V& value);

    iterator find(const K& key);
    iterator find(const V& value);

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    size_t size() const;

    bool empty() const;
    void clear();

private:
    // FIXME unordered_map不支持通过哈希值找到key, 所以需要自己实现HashMap以支持此接口
    std::unordered_map<K, V>                m_storage;
    std::unordered_map<uint64_t, uint64_t>  m_indexMap; // std::hash<V> -> std::hash<K>
};

} // namespace eular

#include "bimap.hpp"

#endif // __EULAR_UTILS_BIMAP_H__
