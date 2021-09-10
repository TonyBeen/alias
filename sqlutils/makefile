CC := g++
STATIC_LIB := /usr/local/lib/libmysqlclient.a
SO_DEPENDENCE :=  -laliaslog -lutils -lpthread -ldl
# -lcrypto -lssl  -lz -lm -lrt -latomic	之前的依赖
SO_FLAGS := -fPIC
CFLAGS := -std=c++11

src := mysql_conn.cpp
headerfile := $(wildcard ./*.h)
obj := $(patsubst %.cpp, %.o, $(src))

main := main.cpp
mainobj := main.o

DESTDIR := /usr/local

all : insall uninstall 

test : $(obj) $(mainobj)
	$(CC) $^ -o $@ $(CFLAGS) $(STATIC_LIB) $(SO_DEPENDENCE)
	-rm -rf $(obj) $(mainobj)

libsqlutils.so : $(obj)
	$(CC) $^ -o $@ -shared $(STATIC_LIB) $(SO_DEPENDENCE)

%.o : %.cpp
	$(CC) $^ -c -o $@ $(CFLAGS) $(SO_FLAGS) $(SO_DEPENDENCE)

insall: libsqlutils.so
	-sudo mv ./libsqlutils.so $(DESTDIR)/lib
	-if [ ! -d "$(DESTDIR)/include/sqlutils/" ]; then sudo mkdir $(DESTDIR)/include/sqlutils/; fi
	-sudo cp $(headerfile) $(DESTDIR)/include/sqlutils/
	-make clean
	-sudo ldconfig

uninstall:
	sudo rm -rf $(DESTDIR)/lib/libsqlutils.so

.PHONY:	all clean test

clean:
	-rm -rf $(obj) $(mainobj) test
