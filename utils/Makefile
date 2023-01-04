INCLUDE_DIR := -I./include
cppflags := -std=c++11
soflags := -fPIC
shared_lib := -lpthread
cc := g++
destfile := /usr/local

src := $(wildcard ./src/*.cpp)
headerfile := $(wildcard ./include/*.h)
headerfile += $(wildcard ./include/*.hpp)
obj := $(patsubst %.cpp, %.o, $(src))

libutils.so : $(obj)
	$(cc) $^ -o $@ -shared $(shared_lib)

%.o : %.cpp
	$(cc) $^ -c -o $@ $(cppflags) $(soflags) $(INCLUDE_DIR)

debug:
	@echo $(src)
	@echo $(obj)
	@echo $(headerfile)

.PHONY:clean install uninstall

install : libutils.so
	-sudo mv ./libutils.so $(destfile)/lib
	-sudo ldconfig
	-if [ ! -d "$(destfile)/include/utils/" ]; then sudo mkdir $(destfile)/include/utils/; fi
	-sudo cp ./include/* $(destfile)/include/utils/
	-make clean

uninstall:
	-rm -rf $(obj)
	-sudo rm -rf $(destfile)/lib/libutils.so
	-sudo rm -rf $(destfile)/include/utils

clean:
	-rm -rf $(obj)