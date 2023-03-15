/*************************************************************************
    > File Name: test_yaml_config.cpp
    > Author: hsz
    > Brief:
    > Created Time: Sat 31 Dec 2022 05:37:15 PM CST
 ************************************************************************/

#include <iostream>
#include "../YamlConfig.h"
#include <gtest/gtest.h>

using namespace std;
using namespace eular;

eular::YamlReader reader;

static std::string file = "./yaml_config.yaml";

TEST(YamlConfigTest, test_loadYaml) {
    reader.loadYaml(file);
    EXPECT_TRUE(reader.valid());
}

TEST(YamlConfigTest, test_lookup) {
    EXPECT_EQ(reader.lookup<std::string>("spec.selector.app"), "nginx");
    EXPECT_EQ(reader.lookup<bool>("log.sync"), true);
    EXPECT_EQ(reader.lookup<int>("log.int"), 10);
    EXPECT_EQ(reader.lookup<double>("log.double"), 3.1415);
}

TEST(YamlConfigTest, test_at) {
    YamlValue value = reader.at("spec.volum.containers");
    if (value.IsSequence()) {
        for (int i = 0; i < value.size(); ++i) {
            const eular::YamlValue &node = value[i];
            if (node.IsMap()) {
                for (auto it = node.begin(); it != node.end(); ++it) {
                    std::cout << it->first.Scalar() << ", " << it->second.Scalar() << std::endl;
                }
            }
        }
    }
}

TEST(YamlConfigTest, test_foreach) {
    reader.foreach(false);
}

int main(int argc, char **argv)
{
    if (argc == 2) {
        file = argv[1];
    }

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
