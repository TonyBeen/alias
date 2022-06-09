cflags := -std=c++11
soflags := -fPIC
shared_lib := -lunwind -lpthread
cc := g++
destfile := /usr/local

src := $(wildcard log*.cpp)
src += callstack.cpp
headerfile := $(wildcard log*.h)
headerfile += callstack.h
obj := $(patsubst %.cpp, %.o, $(src))

mainsrc := testlog.cc
mainobj := testlog.o

testlog : $(obj) $(mainobj)
	$(cc) $^ -o $@ $(cflags) $(shared_lib)
	-rm -rf $(obj) $(mainobj)

liblog.so : $(obj)
	$(cc) $(obj) -o $@ $(cflags) -shared $(shared_lib)

logcat : logcat.cc
	g++ logcat.cc -o logcat -std=c++11

%.o : %.cpp
	$(cc) $^ -c -o $@ $(cflags) $(soflags)

debug:
	@echo $(src)
	@echo $(obj)
	@echo $(headerfile)

.PHONY:clean uninstall install

install : liblog.so logcat
	-(sudo mv ./liblog.so $(destfile)/lib)
	-(sudo ldconfig)
	-(if [ ! -d "$(destfile)/include/log/" ]; then sudo mkdir $(destfile)/include/log/; fi)
	-(sudo cp $(headerfile) $(destfile)/include/log/)
	-(sudo make clean)
	-(sudo mv logcat /usr/bin)

uninstall:
	-rm -rf $(obj) $(mainobj) testlog
	-(sudo rm -rf /usr/lib/liblog.so)
	-(sudo rm -rf /usr/include/log/)

clean:
	-rm -rf $(obj) $(mainobj) testlog
