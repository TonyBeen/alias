SRC := threadpool.cpp
OBJ := threadpool.o

TEST_SRC := test_threadloop.cc

LIB := -lutils -llog -lpthread

CPPFLAG := -std=c++11 -g

LIB_PATH := /usr/local/lib/

all:
	make test_threadloop
	make libthreadpool.a

install:
	make libthreadpool.a
	sudo mv libthreadpool.a $(LIB_PATH)
	make clean

test_threadloop: $(SRC) $(TEST_SRC)
	g++ $^ -o $@ $(LIB) $(CPPFLAG)

libthreadpool.a : $(OBJ)
	ar rcs $@ $^

$(OBJ) : $(SRC)
	g++ $^ -c -o $@ $(CPPFLAG)

.PHONY: all install clean test_threadloop

clean:
	-rm -rf $(OBJ) test_threadloop