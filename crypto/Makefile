CC := g++
CXXFLAGS := -std=c++11 -W -Wall -Werror -Wextra -O2 -m64 -g
CXX_NO_ERROR_FLAGS := -Wno-deprecated -Wno-unused-result -Wno-unused-function -Wno-unused-parameter
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

TARGET := libeularcrypto.so

$(TARGET) : $(OBJLIST)
	$(CC) $^ -o $@ -shared $(LIBPATH) $(LIBLIST)
	rm -rf $(OBJLIST)

install : $(TARGET)
	-(sudo mv $(TARGET) $(DESTPATH)/lib)
	-(sudo ldconfig)
	-(if [ ! -d "$(DESTPATH)/include/crypto/" ]; then sudo mkdir $(DESTPATH)/include/crypto/; fi)
	-(sudo cp $(HEADERFILE) $(DESTPATH)/include/crypto/)
	-(sudo make clean)

uninstall :
	-(sudo rm -rf $(DESTPATH)/lib/$(TARGET))
	-(sudo rm -rf $(DESTPATH)/include/crypto)

test: test/testmain

test/testmain : $(TESTOBJLIST) $(OBJLIST)
	$(CC) $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(CXX_NO_ERROR_FLAGS) $(INCLUDEPATH) $(LIBPATH) $(GTEST_LIB) $(LIBLIST)

%.o : %.cpp
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(CXX_NO_ERROR_FLAGS) $(INCLUDEPATH) $(SOFLAGS)

%.o : %.cc
	$(CC) -c $^ -o $@ $(CXXFLAGS) $(NO_DEPRECATED_WARN) $(CXX_NO_ERROR_FLAGS) $(INCLUDEPATH) $(SOFLAGS)

.PHONY: install uninstall clean $(TARGET) \
		test test/testmain

clean :
	-rm -rf $(OBJLIST) $(TESTOBJLIST)
	-rm -rf test/testmain