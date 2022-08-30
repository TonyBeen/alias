cppflags := -std=c++11 -g
shared_lib := -lunwind -llog -lutils -lpthread
cc := g++

all : 
	make testbuffer
	make testmutex
	make testrwmutex
	make teststring8
	make testthread
	make teststring8_2
	make testtimer
	make testsingleton
	make testutils
	make testbitmap

memcheck:testbuffer testmutex testrwmutex teststring8 \
	testthread teststring8_2 testtimer testsingleton testutils testbitmap
	valgrind --xml=yes --xml-file=./testbuffer_mem_check.xml  --leak-check=full ./testbuffer
	valgrind --xml=yes --xml-file=./testmutex_mem_check.xml  --leak-check=full ./testmutex
	valgrind --xml=yes --xml-file=./testrwmutex_mem_check.xml  --leak-check=full ./testrwmutex
	valgrind --xml=yes --xml-file=./testteststring8_mem_check.xml  --leak-check=full ./teststring8
	valgrind --xml=yes --xml-file=./testthread_mem_check.xml  --leak-check=full ./testthread
	valgrind --xml=yes --xml-file=./testtimer_mem_check.xml  --leak-check=full ./testtimer
	valgrind --xml=yes --xml-file=./testsingleton_mem_check.xml  --leak-check=full ./testsingleton
	valgrind --xml=yes --xml-file=./testutils_mem_check.xml  --leak-check=full ./testutils
	valgrind --xml=yes --xml-file=./testbitmap_mem_check.xml  --leak-check=full ./testbitmap

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

testbitmap : test_bitmap.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

.PHONY : testbuffer testmutex teststring8 testthread testcallstack testutils \
	testtimer testrwmutex testsingleton testrbtree clean all

clean :
	-rm -rf testbuffer testmutex teststring8 testthread teststring8_2 testutils \
		testcallstack testtimer testrwmutex testsingleton testrbtree