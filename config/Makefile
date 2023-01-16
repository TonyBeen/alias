PWD := $(shell pwd)
INCLUDE_DIR := -I$(PWD)
CPPFLAGS  := -std=c++11 # -m64
LIBFLAGS  := -fPIC
LIB_LIST  := -ljsoncpp -lyaml-cpp -lpthread
JSON_LIB  := /usr/local/lib/libjsoncpp.a
YAML_LIB  := /usr/local/lib/libyaml-cpp.a
GTEST_LIB := /usr/local/lib/libgtest.a

DEST_PATH := /usr/local
DEST_INC  := $(DEST_PATH)/include/config
DEST_LIB  := $(DEST_PATH)/lib

CC := g++

SRC_LIST = $(wildcard $(PWD)/*.cpp)
OBJ_LIST = $(patsubst %.cpp, %.o, $(SRC_LIST))

HEADER_FILE_LIST = 	\
	IniConfig.h		\
	JsonConfig.h	\
	typecast.h		\
	XmlConfig.h		\
	YamlConfig.h	\

GEN_BIN := if [ ! -d bin ]; then mkdir bin; fi

TARGET := bin/libconfig.so

all:
	$(GEN_BIN)
	make $(TARGET)

install: all
	if [ ! -d $(DEST_INC) ]; then mkdir $(DEST_INC); fi
	cp $(HEADER_FILE_LIST) $(DEST_INC)
	mv $(TARGET) $(DEST_LIB)

uninstall:
	rm -rf $(DEST_INC)
	rm $(DEST_LIB)/libconfig.so

$(TARGET) : $(OBJ_LIST)
	$(CC) $^ -o $@ -shared $(LIB_LIST)

%.o : %.cpp
	$(CC) $^ -c -o $@ $(CPPFLAGS) $(LIBFLAGS) $(INCLUDE_DIR)

bin/test_yaml_config : test/test_yaml_config.cpp YamlConfig.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) $(YAML_LIB) $(GTEST_LIB) -lpthread

bin/test_json_config: test/test_json_config.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) $(JSON_LIB) $(GTEST_LIB) -lpthread

bin/test_xml_config: test/test_xml_config.cpp XmlConfig.cpp tinyxml2.cpp rwmutex.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) -lpthread

bin/test_ini_config: test/test_ini_config.cpp IniConfig.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR)

bin/test_rwmutex: test/test_rwmutex.cpp rwmutex.cpp
	$(GEN_BIN)
	$(CC) $^ -o $@ $(INCLUDE_DIR) -lpthread

.PHONY: all clean install uninstall \
	bin/test_yaml_config	\
	bin/test_json_config	\
	bin/test_xml_config		\
	bin/test_rwmutex		\

clean:
	rm -rf bin $(OBJ_LIST) $(TARGET)