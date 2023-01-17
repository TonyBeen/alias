YAML_CXX_FLAGS = -fPIC -Wall -Wextra -Wshadow -Weffc++ \
	-Wno-long-long -pedantic -pedantic-errors -std=gnu++11

YAML_CXX_DEFINES = -DYAML_CPP_NO_CONTRIB

YAML_CXX_INCLUDES = -I$(PWD)/3rd_party/yamlcpp/include/

YAML_CXX_INCLUDE_PATH = $(PWD)/3rd_party/yamlcpp/include/yaml-cpp

YAML_CXX_SRC_LIST = $(wildcard $(PWD)/3rd_party/yamlcpp/src/*.cpp)

YAML_CXX_OBJ_LIST = $(patsubst %.cpp, %.o, $(YAML_CXX_SRC_LIST))

$(PWD)/3rd_party/yamlcpp/src/%.o : $(PWD)/3rd_party/yamlcpp/src/%.cpp
	$(CC) $^ -c -o $@ $(YAML_CXX_DEFINES) $(YAML_CXX_FLAGS) $(YAML_CXX_INCLUDES)