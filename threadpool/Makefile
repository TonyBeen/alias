SRC := threadpool.cpp

TEST_SRC := test_threadloop.cc

LIB := -lutils -llog -lpthread

CPPFLAG := -std=c++11 -g

all:
	make test_threadloop

test_threadloop: $(SRC) $(TEST_SRC)
	g++ $^ -o $@ $(LIB) $(CPPFLAG)


.PHONY: all clean test_threadloop

clean:
