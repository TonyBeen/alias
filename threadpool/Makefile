SRC := threadpool.cpp
OBJ := threadpool.o

TEST_SRC := test_threadloop.cc

LIB := -lutils -llog -lpthread

CPPFLAG := -std=c++11 -g

HEADER := threadpool.h

LIB_PATH := /usr/local/lib
INCLUDE_PATH := /usr/local/include

all:
	make test_threadloop
	make libthreadpool.a
	rm -rf $(OBJ)

install:
	-make libthreadpool.a
	-sudo mv libthreadpool.a $(LIB_PATH)
	-if [ ! -d "$(INCLUDE_PATH)/threadpool/" ]; then sudo mkdir $(INCLUDE_PATH)/threadpool/; fi
	-sudo cp -a $(HEADER) $(INCLUDE_PATH)/threadpool/
	-make clean

uninstall:
	sudo rm -rf $(LIB_PATH)/libthreadpool.a

test_threadloop: $(SRC) $(TEST_SRC)
	g++ $^ -o $@ $(LIB) $(CPPFLAG)

libthreadpool.a : $(OBJ)
	ar rcs $@ $^

$(OBJ) : $(SRC)
	g++ $^ -c -o $@ $(CPPFLAG)

.PHONY: all install clean test_threadloop

clean:
	-rm -rf $(OBJ) test_threadloop