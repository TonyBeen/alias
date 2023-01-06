PWD := $(shell pwd)
INCLUDE_DIR := -I$(PWD)
CPPFLAGS  := -std=c++11
LIBFLAGS  := -fPIC
LIB_LIST  := -ljsoncpp -lyaml-cpp -lpthread
JSON_LIB  := /usr/local/lib/libjsoncpp.a
YAML_LIB  := /usr/local/lib/libyaml-cpp.a
GTEST_LIB := /usr/local/lib/libgtest.a
CC := g++
DEST := /usr/local

SRC_LIST = $(wildcard $(PWD)/*.cpp)
OBJ_LIST = $(patsubst %.cpp, %.o, $(SRC_LIST))

HEADER_FILE_LIST = $(wildcard ./*.h)
HEADER_FILE_LIST += $(wildcard ./*.hpp)

GEN_BIN := if [ ! -d bin ]; then mkdir bin; fi

TARGET := bin/libconfig.so

all:
	$(GEN_BIN)
	make $(TARGET)

$(TARGET) : $(OBJ_LIST)
	$(CC) $^ -o $@ -shared $(LIB_LIST)

%.o : %.cpp
	$(CC) $^ -c -o $@ $(CPPFLAGS) $(LIBFLAGS) $(INCLUDE_DIR)

bin/test_yaml_config : test/test_yaml_config.cpp YamlConfig.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) $(YAML_LIB) $(GTEST_LIB) $(LIB_LIST)

bin/test_json_config: test/test_json_config.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) $(JSON_LIB) $(GTEST_LIB) $(LIB_LIST)

.PHONY: all clean \
	bin/test_yaml_config \
	bin/test_json_config

clean:
	rm -rf bin $(OBJ_LIST) $(TARGET)