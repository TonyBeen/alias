/*************************************************************************
    > File Name: test_json_config.cpp
    > Author: hsz
    > Brief:
    > Created Time: Wed 04 Jan 2023 10:07:03 AM CST
 ************************************************************************/

#include <iostream>
#include "../JsonConfig.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

static std::string file = "./json_config.json";

TEST(JsonConfigTest, testRead) {
    eular::JsonReader reader;
    eular::JsonValue jsonValueRoot;
    std::fstream in(file, std::ios::in);
    EXPECT_TRUE(in.is_open());
    std::string json;
    std::stringstream ss;
    ss << in.rdbuf();
    json = ss.str();

    reader.parse(json, jsonValueRoot);
    EXPECT_TRUE(jsonValueRoot["key1"].isNull());
    EXPECT_EQ(jsonValueRoot["key2"].asInt(), 7);
    EXPECT_EQ(jsonValueRoot["key3"].asDouble(), 3.1415);
    EXPECT_EQ(jsonValueRoot["key4"].asString(), "value");
    EXPECT_TRUE(jsonValueRoot["key5"].asBool());
    EXPECT_TRUE(jsonValueRoot["friends"].isArray());
    EXPECT_TRUE(jsonValueRoot["players"].isObject());

    eular::JsonValue &friendValue = jsonValueRoot["friends"];
    for (int i = 0; i < friendValue.size(); ++i) {
        std::cout << friendValue[i].asString() << std::endl;
    }

    eular::JsonValue &playerValue = jsonValueRoot["players"];
    // std::cout << eular::JsonFastWriter().write(playerValue) << std::endl;
    EXPECT_EQ(playerValue["one"], "Kante");
    EXPECT_EQ(playerValue["two"], "Hazard");
    eular::JsonMembers members = playerValue.getMemberNames();
    for (auto name : members) {
        std::cout << playerValue[name].asString() << std::endl;
    }
}

TEST(JsonConfigTest, testParse) {
    eular::JsonReader reader;
    eular::JsonValue jsonValueRoot;
    std::fstream in(file, std::ios::in);
    EXPECT_TRUE(in.is_open());
    std::string json;
    std::stringstream ss;
    ss << in.rdbuf();
    json = ss.str();

    reader.parse(json, jsonValueRoot);

    auto members = jsonValueRoot.getMemberNames();
    eular::JsonValue defaultValue;
    for (auto name : members) {
        const eular::JsonValue &value = jsonValueRoot.get(name, defaultValue);
        if (value.isNull()) {
            continue;
        }
        EXPECT_TRUE(value != defaultValue);
        std::cout << name << ", type = " << value.type() << std::endl;

        if (value.isInt() || value.isInt64()) {
            std::cout << "\tvalue = " << value.asInt() << std::endl;
        } else if (value.isUInt() || value.isUInt64()) {
            std::cout << "\tvalue = " << value.asUInt() << std::endl;
        } else if (value.isDouble()) {
            std::cout << "\tvalue = " << value.asDouble() << std::endl;
        } else if (value.isString()) {
            std::cout << "\tvalue = " << value.asString() << std::endl;
        } else if (value.isBool()) {
            std::cout << "\tvalue = " << value.asBool() << std::endl;
        } else if (value.isArray()) {
            for (int i = 0; i < value.size(); ++i) {
                std::cout << value[i].asString() << "\t";
            }
            std::cout << std::endl;
        } else if (value.isObject()) {
            const eular::JsonMembers &memberNames = value.getMemberNames();
            for (auto str : memberNames) {
                std::cout << "[" << str << ", " << value[str].asString() << "]" << std::endl;
            }
        }
    }
}

TEST(JsonConfigTest, test_write) {
    eular::JsonFastWriter fastWriter;
    eular::JsonStyledWriter styleWriter;
    eular::JsonValue root;

    root["key1"] = eular::JsonValue(eular::JsonValueType::nullValue); // nullValue
    root["key2"] = 7;
    root["key3"] = 3.1415;
    root["key4"] = "value";
    root["key5"] = true;

    eular::JsonValue array; // eular::JsonValue array(eular::JsonValueType::arrayValue);
    array.append("Dammy");
    array.append("Jack");
    root["friends"] = array;

    eular::JsonValue object;
    object["one"] = "Kante";
    object["two"] = "Hazard";
    root["players"] = object;

    std::cout << "FastWriter: \n" << fastWriter.write(root) << std::endl;
    std::cout << "StyledWriter: \n" << styleWriter.write(root) << std::endl;
    /**
     * {
     *    "friends" : [ "Dammy", "Jack" ],
     *    "key1" : null,
     *    "key2" : 7,
     *    "key3" : 3.1415000000000002,
     *    "key4" : "value",
     *    "key5" : true,
     *    "players" : {
     *        "one" : "Kante",
     *        "two" : "Hazard"
     *    }
     * }
     * 
     */
}

int main(int argc, char **argv)
{
    if (argc == 2) {
        file = argv[1];
    }

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
