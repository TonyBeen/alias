cppflags := -std=c++11 -g
shared_lib := -lunwind -llog -lutils -lpthread
static_lib := /usr/local/lib/libgtest.a
cc := g++

all : 
	make testbuffer
	make testmutex
	make testmutex_2
	make testrwmutex
	make teststring8
	make testthread
	make testtimer
	make testsingleton
	make testutils
	make testbitmap
	make testtypes
	make testrbtreeapi
	make testmapnode

memcheck:testbuffer testmutex testmutex_2 testrwmutex teststring8 \
	testthread teststring8_2 testtimer testsingleton testutils testbitmap \
	testmapnode testmap
	valgrind --xml=yes --xml-file=./testbuffer_mem_check.xml  --leak-check=full ./testbuffer
	valgrind --xml=yes --xml-file=./testmutex_mem_check.xml  --leak-check=full ./testmutex
	valgrind --xml=yes --xml-file=./testmutex_mem_check.xml  --leak-check=full ./testmutex_2
	valgrind --xml=yes --xml-file=./testrwmutex_mem_check.xml  --leak-check=full ./testrwmutex
	valgrind --xml=yes --xml-file=./testteststring8_mem_check.xml  --leak-check=full ./teststring8
	valgrind --xml=yes --xml-file=./testthread_mem_check.xml  --leak-check=full ./testthread
	valgrind --xml=yes --xml-file=./testtimer_mem_check.xml  --leak-check=full ./testtimer
	valgrind --xml=yes --xml-file=./testsingleton_mem_check.xml  --leak-check=full ./testsingleton
	valgrind --xml=yes --xml-file=./testutils_mem_check.xml  --leak-check=full ./testutils
	valgrind --xml=yes --xml-file=./testbitmap_mem_check.xml  --leak-check=full ./testbitmap
	valgrind --xml=yes --xml-file=./testmapnode_mem_check.xml  --leak-check=full ./testmapnode
	valgrind --xml=yes --xml-file=./testmap_mem_check.xml  --leak-check=full ./testmap
	valgrind --xml=yes --xml-file=./testsharedbuffer_mem_check.xml  --leak-check=full ./testsharedbuffer

testmutex : testmutex.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testmutex_2 : test_mutex_2.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testrwmutex : test_rwmutex.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

teststring8 : teststring8.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

testthread : testthread.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testbuffer : testbuffer.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

testcallstack : testcallstack.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testtimer : test_timer.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testsingleton : test_singleton.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testrbtree : test_rbtree.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testutils : test_utils.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testbitmap : test_bitmap.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

testtypes : test_types.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

testrbtreeapi : test_rbtree_api.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testmapnode : test_map_node.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testmap : test_map.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

testany : test_any.cc
	$(cc) $^ -o $@ $(cppflags) $(shared_lib)

testsharedbuffer : test_sharedbuffer.cc
	$(cc) $^ -o $@ $(cppflags) $(static_lib) $(shared_lib)

.PHONY : testbuffer testmutex teststring8 testthread testcallstack testutils \
		testtimer testrwmutex testsingleton testrbtree testrbtreeapi testmapnode testmap \
		testmutex_2 testany testsharedbuffer \
		clean all

clean :
	-rm -rf testbuffer testmutex teststring8 testthread testutils \
		testcallstack testtimer testrwmutex testsingleton testrbtree testbitmap \
		testrbtreeapi testmapnode testmap testany testsharedbuffer