cppflags := -std=c++11
shared_lib := -lunwind -laliaslog -lutils -lpthread
cc := g++

all : testbuffer testmutex teststring8 testthread teststring8_2

testmutex : testmutex.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

teststring8 : teststring8.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

teststring8_2 : teststring8_2.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testthread : testthread.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testbuffer : testbuffer.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testcallstack : testcallstack.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

.PHONY : testbuffer testmutex teststring8 testthread testcallstack clean

clean :
	-rm -rf testbuffer testmutex teststring8 testthread teststring8_2 testcallstack