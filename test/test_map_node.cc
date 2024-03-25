/*************************************************************************
    > File Name: test_map_node.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 28 Nov 2022 04:29:58 PM CST
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <random>

#include "catch/catch.hpp"
#include "../utils/include/map_node.h"

TEST_CASE("test_map_data_all", "[MapData]") {
    detail::MapData<int32_t, int32_t> *data = detail::MapData<int32_t, int32_t>::create();
    REQUIRE(data != nullptr);
    CHECK(data->size() == 0);

    int32_t insertSize = 10000;
    for (int32_t i = 0; i < insertSize; i++) {
        data->insert(i, i);
    }

    CHECK(insertSize == data->size());

    int32_t value = 0;
    for (auto it = data->begin(); it != data->end(); it = data->nextNode(it)) {
        CHECK(it->key == value);
        CHECK(it->value == value);
        value++;
    }

    int32_t eraseSize = 1000;
    for (int32_t i = 0; i < eraseSize; i++) {
        data->erase(i);
    }
    CHECK(data->size() == (insertSize - eraseSize));

    value = 0;
    for (auto it = data->rbegin(); it != data->rend(); it = data->prevNode(it)) {
        CHECK(it->key == (insertSize - 1 - value));
        CHECK(it->value == (insertSize - 1 - value));
        ++value;
    }

    // 测试插入相同数据时返回nullptr
    detail::MapNode<int32_t, int32_t> *node = data->insert(eraseSize + 1, 100);
    CHECK(node == nullptr);

    node = data->find(eraseSize + 1);
    CHECK(node->value == (eraseSize + 1));

    data->clear();
    CHECK(data->size() == 0);

    for (auto it = data->begin(); it != data->end(); it = data->nextNode(it)) {
        REQUIRE(false);
    }
    detail::MapDataBase::FreeData(data);
}

// 遍历过程中删除节点
TEST_CASE("Delete nodes during traversal", "[MapData]") {
    detail::MapData<int32_t, int32_t> *data = detail::MapData<int32_t, int32_t>::create();
    REQUIRE(data != nullptr);

    int32_t insertSize = 32;
    for (int32_t i = 0; i < insertSize; i++) {
        data->insert(i, i);
    }

    REQUIRE(insertSize == data->size());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, insertSize - 1);
    int randomNumber = dis(gen);

    int32_t value = 0;
    for (auto it = data->begin(); it != data->end(); ++value) {
        if (it->key == randomNumber) {
            it = data->erase(it->key);
            continue;
        }

        CHECK(it->value == value);
        it = data->nextNode(it);
    }

    data->clear();
    detail::MapDataBase::FreeData(data);
}

// 测试随机插入和删除50w数据
void test_insert()
{
    detail::MapData<int32_t, int32_t> *data = detail::MapData<int32_t, int32_t>::create();

    int32_t insertSize = 500000;
    for (int32_t i = 0; i < insertSize; i++) {
        data->insert(i, i);
    }

    data->clear();
    detail::MapDataBase::FreeData(data);
}

void test_erase()
{
    detail::MapData<int32_t, int32_t> *data = detail::MapData<int32_t, int32_t>::create();

    int32_t insertSize = 500000;
    for (int32_t i = 0; i < insertSize; i++) {
        data->insert(i, i);
    }

    data->clear();
    detail::MapDataBase::FreeData(data);
}

TEST_CASE("MapNode Benchmark") {        
    BENCHMARK("Map insert performance") {
        test_insert();
    };
}