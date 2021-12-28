CC := g++
CXXFLAGS := -std=c++11 -g
NO_DEPRECATED_WARN := -Wno-deprecated-declarations # -Wno-error=deprecated-declarations
SOFLAGS := -fPIC

INCLUDEPATH := -I./

LIBPATH := -L/usr/local/lib/
LIBLIST := /usr/local/lib/libcrypto.a -lutils -llog -lpthread -ldl

INCLUDELIST := $(wildcard *.h)
INCLUDELIST += $(wildcard *.hpp)

SRCLIST := $(wildcard *.cpp)
OBJLIST := $(patsubst %.cpp, %.o, $(SRCLIST))

TESTSRCLIST := $(wildcard ./test/*.cc)
TESTOBJLIST := $(patsubst %.cc, %.o, $(TESTSRCLIST))

libeularcrypto.so : $(OBJLIST)
	$(CC) $^ -o $@ -shared $(LIBPATH) $(LIBLIST)
	rm -rf $(OBJLIST)

testall:
	make test/testrsa
	make test/testaes
	make test/testmd5

test/testrsa : test/test_rsa.cc rsa.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(LIBLIST)
test/testaes : test/test_aes.cc aes.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(LIBLIST)
test/testmd5  : test/test_md5.cc md5.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(LIBLIST)
test/testbase64  : test/test_base64.cc base64.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(LIBLIST)

%.o : %.cpp
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(SOFLAGS)
%.o : %.cc
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(SOFLAGS)

.PHONY: install uninstall clean testrsa libeularcrypto.so \
		testall test/testrsa test/testaes test/testaes

clean :
	-rm -rf $(OBJLIST) $(TESTOBJLIST)
	-rm -rf testrsa