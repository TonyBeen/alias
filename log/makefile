cflags := -std=c++11
soflags := -fPIC
shared_lib := -lpthread
cc := g++
destfile := /usr/local

src := $(wildcard log*.cpp)
headerfile := $(wildcard log*.h)
obj := $(patsubst %.cpp, %.o, $(src))

mainsrc := testlog.cc
mainobj := testlog.o

testlog : $(obj) $(mainobj)
	$(cc) $^ -o $@ $(cflags) $(shared_lib)

libaliaslog.so : $(obj)
	$(cc) $(obj) -o $@ $(cflags) -shared $(shared_lib)

%.o : %.cpp
	$(cc) $^ -c -o $@ $(cflags) $(soflags)

debug:
	@echo $(src)
	@echo $(obj)
	@echo $(headerfile)

.PHONY:clean uninstall install

install : libaliaslog.so
	-(sudo mv ./libaliaslog.so $(destfile)/lib)
	-(sudo ldconfig)
	-(if [ ! -d "$(destfile)/include/log/" ]; then sudo mkdir $(destfile)/include/log/; fi)
	-(sudo cp $(headerfile) $(destfile)/include/log/)
	-(sudo make clean)

uninstall:
	-rm -rf $(obj) $(mainobj) testlog
	-(sudo rm -rf /usr/lib/libaliaslog.so)
	-(sudo rm -rf /usr/include/log/)

clean:
	-rm -rf $(obj) $(mainobj) testlog
