INCLUDE_DIR := -I/home/hsz/VScode/alias/utils/include
cppflags := -std=c++11
soflags := -fPIC
shared_lib := -lunwind -laliaslog -lpthread
cc := g++

src := $(wildcard ./src/*.cpp)
headerfile := $(wildcard ./include/*.h)
obj := $(patsubst %.cpp, %.o, $(src))

libutils.so : $(obj)
	$(cc) $(obj) -o $@ $(cppflags) -shared $(shared_lib)

%.o : %.cpp
	$(cc) $^ -c -o $@ $(cppflags) $(soflags) $(INCLUDE_DIR)

debug:
	@echo $(src)
	@echo $(obj)
	@echo $(headerfile)

.PHONY:clean install uninstall

install : libutils.so
	$(shell sudo mv ./libutils.so /usr/lib)
	$(shell sudo ldconfig)

	$(shell if [ ! -d "/usr/include/utils/" ]; then sudo mkdir /usr/include/utils/; fi)
	$(shell sudo cp ./include/* /usr/include/utils/)
	$(shell make clean)
uninstall:
	-rm -rf $(obj)
	$(shell sudo rm -rf /usr/lib/libutils.so)
	$(shell sudo rm -rf /usr/include/utils)
clean:
	-rm -rf $(obj)