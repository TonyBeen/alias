/*************************************************************************
    > File Name: test_any.cc
    > Author: hsz
    > Brief:
    > Created Time: Fri 09 Dec 2022 03:52:20 PM CST
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include <string>

#include "catch/catch.hpp"
#include "utils/any.hpp"

TEST_CASE("std::string -> any -> std::string", "[any]") {
    eular::Any any(std::string("Hello"));
    std::string str = eular::any_cast<std::string>(any);

    REQUIRE(str == "Hello");
}

struct Foo
{
    uint32_t identification;
    uint32_t version;
    uint32_t size;
    char buffer[4];
};

TEST_CASE("Foo -> any -> std::string", "[any]") {

    Foo foo;
    foo.identification = 1;
    foo.version = 2;
    foo.size = 3;
    foo.buffer[0] = 'H';
    foo.buffer[1] = 'e';
    foo.buffer[2] = 'l';

    eular::Any any(foo);

    std::string str = eular::any_cast<std::string>(any);

    // any_cast failed
    REQUIRE(str == std::string());

    // success
    Foo foo2 = eular::any_cast<Foo>(any);
    REQUIRE(foo.identification == foo2.identification);
    REQUIRE(foo.version == foo2.version);
    REQUIRE(foo.size == foo2.size);
}