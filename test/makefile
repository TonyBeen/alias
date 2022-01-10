cppflags := -std=c++11 -g
shared_lib := -lunwind -llog -lutils -lpthread
cc := g++

all : 
	make testbuffer
	make testmutex
	make testrwmutex
	make teststring8
	make testthrea
	make teststring8_2
	make testtimer
	make testsingleton
	make testrbtree
	make testutils

testmutex : testmutex.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testrwmutex : test_rwmutex.cc
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

testtimer : test_timer.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testsingleton : test_singleton.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testrbtree : test_rbtree.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testyaml : testyaml.cc
	g++ testyaml.cc -o testyaml -std=c++11 -lyaml-cpp

testutils : test_utils.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

.PHONY : testbuffer testmutex teststring8 testthread testcallstack testutils \
	testtimer testrwmutex testsingleton testrbtree clean all

clean :
	-rm -rf testbuffer testmutex teststring8 testthread teststring8_2 testutils \
		testcallstack testtimer testrwmutex testsingleton testrbtree