cflags := -std=c++11 -lutils
soflags := -fPIC
shared_lib := -lpthread
cc := g++

src := $(wildcard log*.cpp)
headerfile := $(wildcard log*.h)
obj := $(patsubst %.cpp, %.o, $(src))

mainsrc := testlog.cc
mainobj := testlog.o

testlog : $(obj) $(mainobj)
	$(cc) $^ -o $@ $(cflags) $(shared_lib)

libaliaslog.so : $(obj)
	$(cc) $(obj) -o $@ $(cflags) -shared

%.o : %.cpp
	$(cc) $^ -c -o $@ $(cflags) $(soflags)

debug:
	@echo $(src)
	@echo $(obj)
	@echo $(headerfile)

.PHONY:clean cleanall install

install : libaliaslog.so
	$(shell sudo mv ./libaliaslog.so /usr/lib)
	$(shell sudo ldconfig)
	$(shell if [ ! -d "/usr/include/log/" ]; then sudo mkdir /usr/include/log/; fi)
	$(shell sudo cp $(headerfile) /usr/include/log/)
	$(shell sudo make clean)

cleanall:
	-rm -rf $(obj) $(mainobj) testlog
	$(shell sudo rm -rf /usr/lib/libaliaslog.so)
	$(shell sudo rm -rf /usr/include/log/)

clean:
	-rm -rf $(obj) $(mainobj)
