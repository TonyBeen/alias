/*************************************************************************
    > File Name: test_hash.cc
    > Author: hsz
    > Brief:
    > Created Time: Thu 16 Feb 2023 05:08:44 PM CST
 ************************************************************************/

#include <utils/hash.h>
#include <gtest/gtest.h>
#include <log/callstack.h>

class Key : public eular::HashCmptBase
{
public:
    Key(int v) : h(v) { }
    virtual uint32_t hash() const 
    {
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&h);
        return eular::HashCmptBase::compute(ptr, sizeof(h));
    }

    bool operator==(const Key &o) const { return this->h == o.h; }

    int h{0};
};

TEST(TestHashMap, test_construction) {
    // 测试默认构造函数
    int num = 100;
    eular::HashMap<Key, int> map;
    map.insert(Key(10), num);

    // 测试拷贝构造函数
    decltype(map) map2(map);
    EXPECT_TRUE(map2.size() == map.size());
    {
        const auto &mm = map2;
        EXPECT_TRUE(mm.at(Key(10)) == num);
    }
    
    // 测试初始化列表构造
    eular::HashMap<Key, int> map3 = {{10, 10}, {20, 20}};
    EXPECT_TRUE(map3.size() == 2);

    // 测试移动赋值函数
    map3 = std::move(map); // 此时map3与map2共享
    EXPECT_TRUE(map3.size() == 1);
    EXPECT_TRUE(map.size()== 2);

    // 测试移动构造函数
    decltype(map) map4(std::move(map2));
    EXPECT_TRUE(map4.size() == map3.size());
}

TEST(TestHashMap, test_read_write_foreach_erase) {
    int num = 100;
    int begin = 0;
    int recyle = 10;
    eular::HashMap<Key, int> map1;
    for (int i = begin; i < recyle; ++i) {
        map1.insert(Key(i), num + i);
    }
    EXPECT_TRUE(map1.size() == recyle);

    map1.at(Key(begin)) = num * recyle;
    const auto &v = map1.at(Key(begin));
    EXPECT_TRUE(v == num * recyle);
    map1.at(Key(begin)) = num;

    for (int i = 0; i < recyle; ++i) {
        auto it = map1.find(Key(i));
        assert(it != map1.end());
        printf("hash = %u, %d\n", it.key().hash(), it.value());
        EXPECT_TRUE(it.value() == (num + i));
    }
    printf("\n");

    for (auto it = map1.begin(); it != map1.end(); ++it) {
        
    }
}

void catchSig(int sig)
{
    eular::CallStack stack;
    stack.update();
    stack.log("Stack", eular::LogLevel::LEVEL_FATAL);

    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGSEGV, catchSig);
    testing::InitGoogleTest(&argc, argv); 
	return RUN_ALL_TESTS();
}
