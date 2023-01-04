INCLUDE_DIR := -I.
CPPFLAGS  := -std=c++11
LIBFLAGS  := -fPIC
LIB_LIST  := -lpthread
JSON_LIB  := /usr/local/lib/libjsoncpp.a
YAML_LIB  := /usr/local/lib/libyaml-cpp.a
GTEST_LIB := /usr/local/lib/libgtest.a
CC := g++
DEST := /usr/local

SRC_LIST = $(wildcard ./*.cpp)
OBJ_LIST = $(patsubst %.cpp, %.o, $(src))

HEADER_FILE_LIST = $(wildcard ./*.h)
HEADER_FILE_LIST += $(wildcard ./*.hpp)

GEN_BIN := if [ ! -d "./bin" ]; then mkdir bin; fi

bin/test_yaml_config : test/test_yaml_config.cpp YamlConfig.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(YAML_LIB) $(GTEST_LIB) $(LIB_LIST)

.PHONY: clean \
	bin/test_yaml_config

clean:
	rm -rf test/test_yaml_config