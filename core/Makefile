SRCROOT = $(realpath ../)
include $(SRCROOT)/config.mak

TARGET = libabspell.so

SOURCES = $(SRCROOT)/core/*.cc
HEADERS = $(INCLUDES_PATH)/*.h

CXXFLAGS += -shared -fPIC

.PHONY: clean check all

all : $(INSTALL_LIB_PATH) $(INSTALL_LIB_PATH)/$(TARGET)
	$(MAKE) -C test all

$(INSTALL_LIB_PATH):
	mkdir $(INSTALL_LIB_PATH)

$(INSTALL_LIB_PATH)/$(TARGET) : $(SOURCES) $(HEADERS)
	$(CXX) -o $(INSTALL_LIB_PATH)/$(TARGET) $(CXXFLAGS) $(SOURCES) 

clean : 
	rm -rf $(INSTALL_LIB_PATH)/$(TARGET)

check :
	$(MAKE) -C test check
