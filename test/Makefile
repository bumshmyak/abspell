SRCROOT = $(realpath ../)
include $(SRCROOT)/config.mak

.PHONY: clean check all test

CC_SOURCES = $(wildcard *.cc)
HEADERS = $(INCLUDES_PATH)/*.h

all: FIND-GTEST

GTEST_CONFIG = gtest-config

FIND-GTEST:
	$(GTEST_CONFIG) --min-version=1.0; [ $$? -ne 0 ] || $(MAKE) googletest


TARGET = googletest
OBJS = $(subst .cc,.o,$(CC_SOURCES))

$(TARGET): gtest-main.o $(OBJS)
	g++ $(shell $(GTEST_CONFIG) --ldflags --libs) -o $(TARGET) gtest-main.o $(OBJS) -labspell -L$(INSTALL_LIB_PATH) 

%.o : %.cc
	g++ $(shell $(GTEST_CONFIG) --cppflags --cxxflags) -o $@ -c $< -I$(INCLUDES_PATH) 

gtest-main.o: gtest_main.cpp
	g++ $(shell $(GTEST_CONFIG) --cppflags --cxxflags) -o gtest-main.o -c gtest_main.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)

test:
	LD_LIBRARY_PATH=${LDPATH}:$(INSTALL_LIB_PATH) ./$(TARGET)
