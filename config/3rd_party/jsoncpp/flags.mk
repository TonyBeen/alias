JSON_CXX_FLAGS = -O3 -DNDEBUG -fPIC -Wall -Wconversion \
	-Wshadow -Wextra -Wpedantic -Werror=strict-aliasing -std=c++11

JSON_CXX_DEFINES = -DJSON_DLL_BUILD -Djsoncpp_lib_EXPORTS

JSON_CXX_INCLUDES = -I$(PWD)/3rd_party/jsoncpp/include/

JSON_CXX_INCLUDE_PATH = $(PWD)/3rd_party/jsoncpp/include/json/

JSON_CXX_SRC_LIST = $(wildcard $(PWD)/3rd_party/jsoncpp/src/*.cpp)

JSON_CXX_OBJ_LIST = $(patsubst %.cpp, %.o, $(JSON_CXX_SRC_LIST))

$(PWD)/3rd_party/jsoncpp/src/%.o : $(PWD)/3rd_party/jsoncpp/src/%.cpp
	$(CC) $^ -c -o $@ $(JSON_CXX_DEFINES) $(JSON_CXX_FLAGS) $(JSON_CXX_INCLUDES)