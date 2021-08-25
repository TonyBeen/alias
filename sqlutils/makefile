CC := g++
SO_DIR := -L./
SO_LIB := libmysqlclient.a
SO_DEPENDENCE := -lssl -laliaslog -lutils -lpthread -lcrypto -ldl -lz
SO_FLAGS := -fPIC
CFLAGS := -std=c++11

src := main.cpp sql_conn.cpp
obj := $(patsubst %.cpp, %.o, $(src))

all : insall

test : $(obj)
	$(CC) $^ -o $@ $(CFLAGS) $(SO_DIR) $(SO_LIB) $(SO_DEPENDENCE)

libsqlutils.so : $(obj)
	$(CC) $^ -o $@ $(CFLAGS) $(SO_FLAGS) $(SO_DIR) $(SO_LIB) $(SO_DEPENDENCE)

%.o : %.cpp
	$(CC) $^ -c -o $@ $(CFLAGS)

insall:
	


.PHONY:	all clean test

clean:
	-rm -rf $(obj) test
