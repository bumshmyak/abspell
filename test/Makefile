SRCROOT = $(realpath ../)
include $(SRCROOT)/config.mak

.PHONY: clean check all test

TARGET = googletest
CC_SOURCES = $(wildcard *.cc)
HEADERS = $(INCLUDES_PATH)/*.h

all: $(TARGET)


OBJS = $(subst .cc,.o,$(CC_SOURCES))

$(TARGET): gtest-main.o $(OBJS)
	g++ $(shell gtest-config --ldflags --libs) -o $(TARGET) gtest-main.o $(OBJS)

%.o : %.cc
	g++ $(shell gtest-config --cppflags --cxxflags) -o $@ -c $< -I$(INCLUDES_PATH) 

gtest-main.o: gtest_main.cpp
	g++ $(shell gtest-config --cppflags --cxxflags) -o gtest-main.o -c gtest_main.cpp

clean:
	rm -f *.o
	rm $(TARGET)

test:
	./$(TARGET)
