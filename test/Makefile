CPP_FLAGS := -std=c++11 -g
SHARED_LIB := -lunwind -llog -lutils -lpthread
STATIC_LIB := /usr/local/lib/libgtest.a
CC := g++

GEN_BIN := if [ ! -d "./bin" ]; then mkdir bin; fi
GEN_XML := if [ ! -d "./xml" ]; then mkdir xml; fi

all : 
	make bin/testbuffer
	make bin/testmutex
	make bin/testmutex_2
	make bin/testrwmutex
	make bin/teststring8
	make bin/testthread
	make bin/testtimer
	make bin/testsingleton
	make bin/testutils
	make bin/testbitmap
	make bin/testtypes
	make bin/testrbtreeapi
	make bin/testmapnode
	make bin/testhash

memcheck: bin/testbuffer bin/testmutex bin/testmutex_2 bin/testrwmutex bin/teststring8 \
	bin/testthread bin/teststring8_2 bin/testtimer bin/testsingleton bin/testutils bin/testbitmap \
	bin/testmapnode bin/testmap make bin/testhash
	$(GEN_XML)
	valgrind --xml=yes --xml-file=./testbuffer_mem_check.xml  --leak-check=full ./bin/testbuffer
	valgrind --xml=yes --xml-file=./testmutex_mem_check.xml  --leak-check=full ./bin/testmutex
	valgrind --xml=yes --xml-file=./testmutex_mem_check.xml  --leak-check=full ./bin/testmutex_2
	valgrind --xml=yes --xml-file=./testrwmutex_mem_check.xml  --leak-check=full ./bin/testrwmutex
	valgrind --xml=yes --xml-file=./testteststring8_mem_check.xml  --leak-check=full ./bin/teststring8
	valgrind --xml=yes --xml-file=./testthread_mem_check.xml  --leak-check=full ./bin/testthread
	valgrind --xml=yes --xml-file=./testtimer_mem_check.xml  --leak-check=full ./bin/testtimer
	valgrind --xml=yes --xml-file=./testsingleton_mem_check.xml  --leak-check=full ./bin/testsingleton
	valgrind --xml=yes --xml-file=./testutils_mem_check.xml  --leak-check=full ./bin/testutils
	valgrind --xml=yes --xml-file=./testbitmap_mem_check.xml  --leak-check=full ./bin/testbitmap
	valgrind --xml=yes --xml-file=./testmapnode_mem_check.xml  --leak-check=full ./bin/testmapnode
	valgrind --xml=yes --xml-file=./testmap_mem_check.xml  --leak-check=full ./bin/testmap
	valgrind --xml=yes --xml-file=./testsharedbuffer_mem_check.xml  --leak-check=full ./bin/testsharedbuffer
	valgrind --xml=yes --xml-file=./testhash_mem_check.xml  --leak-check=full ./bin/testhash

bin/testmutex : testmutex.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testmutex_2 : test_mutex_2.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testrwmutex : test_rwmutex.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/teststring8 : teststring8.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testthread : testthread.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testbuffer : testbuffer.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testcallstack : testcallstack.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testtimer : test_timer.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testsingleton : test_singleton.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testrbtree : test_rbtree.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testutils : test_utils.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testbitmap : test_bitmap.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testtypes : test_types.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testrbtreeapi : test_rbtree_api.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testmapnode : test_map_node.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testmap : test_map.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testany : test_any.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(SHARED_LIB)

bin/testsharedbuffer : test_sharedbuffer.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

bin/testhash : test_hash.cc
	$(GEN_BIN)
	$(CC) $^ -o $@ $(CPP_FLAGS) $(STATIC_LIB) $(SHARED_LIB)

.PHONY : clean all 		\
	bin/testbuffer 		\
	bin/testmutex 		\
	bin/testmutex_2		\
	bin/testrwmutex		\
	bin/teststring8		\
	bin/teststring8_2 	\
	bin/testthread 		\
	bin/testtimer 		\
	bin/testsingleton 	\
	bin/testutils 		\
	bin/testbitmap 		\
	bin/testrbtree		\
	bin/testrbtreeapi	\
	bin/testmapnode 	\
	bin/testmap			\
	bin/testsharedbuffer \
	bin/testhash		\


clean :
	-rm -rf bin xml