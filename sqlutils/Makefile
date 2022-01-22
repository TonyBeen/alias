CC := g++
STATIC_LIB := /usr/local/lib/libmysqlclient.a /usr/local/lib/libhiredis.a
SO_DEPENDENCE :=  -llog -lutils -lpthread -ldl
# -lcrypto -lssl  -lz -lm -lrt -latomic	之前的依赖
SO_FLAGS := -fPIC
CPPFILAGS := -std=c++11

SRCLIST := $(wildcard ./*.cpp)
HEADERFILE := $(wildcard ./*.h)
OBJLIST := $(patsubst %.cpp, %.o, $(SRCLIST))

DESTDIR := /usr/local

all : insall uninstall

libsqlutils.so : $(OBJLIST)
	$(CC) $^ -o $@ -shared $(STATIC_LIB) $(SO_DEPENDENCE)

%.o : %.cpp
	$(CC) $^ -c -o $@ $(CPPFILAGS) $(SO_FLAGS) $(SO_DEPENDENCE)

insall: libsqlutils.so
	-sudo mv ./libsqlutils.so $(DESTDIR)/lib
	-if [ ! -d "$(DESTDIR)/include/sqlutils/" ]; then sudo mkdir $(DESTDIR)/include/sqlutils/; fi
	-sudo cp $(HEADERFILE) $(DESTDIR)/include/sqlutils/
	-make clean
	-sudo ldconfig

uninstall:
	sudo rm -rf $(DESTDIR)/lib/libsqlutils.so
	sudo rm -rf $(DESTDIR)/include/sqlutils

testmysql : test_mysql.cc mysql.cpp
	$(CC) $^ -o $@ $(CPPFILAGS) $(STATIC_LIB) $(SO_DEPENDENCE)
testredis : test_redis.cc redis.cpp
	$(CC) $^ -o $@ $(CPPFILAGS) $(STATIC_LIB) $(SO_DEPENDENCE)

.PHONY:	all clean testmysql testredis

clean:
	-rm -rf $(OBJLIST) testmysql testredis
