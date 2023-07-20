CC := g++
CXXFLAGS := -std=c++11 -g
NO_DEPRECATED_WARN := -Wno-deprecated-declarations # -Wno-error=deprecated-declarations
SOFLAGS := -fPIC

DESTPATH := /usr/local
HEADERFILE := $(wildcard *.h)

INCLUDEPATH := -I./

LIBPATH := -L/usr/local/lib/
LIBLIST := /usr/local/lib/libcrypto.a -lutils -llog -lpthread -ldl
GTEST_LIB := /usr/local/lib/libgtest.a

INCLUDELIST := $(HEADERFILE)
INCLUDELIST += $(wildcard *.hpp)

SRCLIST := $(wildcard *.cpp)
OBJLIST := $(patsubst %.cpp, %.o, $(SRCLIST))

TESTSRCLIST := $(wildcard ./test/*.cc)
TESTOBJLIST := $(patsubst %.cc, %.o, $(TESTSRCLIST))

install : libeularcrypto.so
	-(sudo mv ./libeularcrypto.so $(DESTPATH)/lib)
	-(sudo ldconfig)
	-(if [ ! -d "$(DESTPATH)/include/crypto/" ]; then sudo mkdir $(DESTPATH)/include/crypto/; fi)
	-(sudo cp $(HEADERFILE) $(DESTPATH)/include/crypto/)
	-(sudo make clean)

uninstall :
	-(sudo rm -rf $(DESTPATH)/lib/libeularcrypto.so)
	-(sudo rm -rf $(DESTPATH)/include/crypto)

libeularcrypto.so : $(OBJLIST)
	$(CC) $^ -o $@ -shared $(LIBPATH) $(LIBLIST)
	rm -rf $(OBJLIST)

testall:
	make test/testrsa
	make test/testaes
	make test/testmd5
	make test/testbase64

test/testrsa : test/test_rsa.cc rsa.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)
test/testaes : test/test_aes.cc aes.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)
test/testmd5 : test/test_md5.cc md5.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)
test/testbase64 : test/test_base64.cc base64.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)
test/testcrc32 : test/test_crc32.cc crc32.cpp
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)

%.o : %.cpp
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(SOFLAGS)
%.o : %.cc
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(INCLUDEPATH) $(SOFLAGS)

.PHONY: install uninstall clean testrsa libeularcrypto.so \
		testall test/testrsa test/testaes test/testaes

clean :
	-rm -rf $(OBJLIST) $(TESTOBJLIST)
	-rm -rf test/testrsa test/testaes test/testmd5 test/testbase64