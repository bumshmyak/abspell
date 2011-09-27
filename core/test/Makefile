SRCROOT = $(realpath ../..)
include $(SRCROOT)/config.mak

TARGET = test 

SOURCES = $(SRCROOT)/core/test/*.cc
HEADERS = $(INCLUDES_PATH)/*.h

LDFLAGS += -L$(INSTALL_LIB_PATH) -labspell
LDFLAGS += $(GTEST_LIB_PATH)/gtest_main.a 
LDFLAGS += -Wl,-rpath=$(INSTALL_LIB_PATH)

CXXFLAGS += -I$(GTEST_INCLUDE_PATH)


.PHONY: all clean check

all : $(OUTPUT_BIN_DIR)/$(TARGET)

$(OUTPUT_BIN_DIR)/$(TARGET) : $(OUTPUT_BIN_DIR) $(SOURCES) $(HEADERS)
	$(CXX) -o $(OUTPUT_BIN_DIR)/$(TARGET) $(LDFLAGS) $(CXXFLAGS) $(SOURCES)

$(OUTPUT_BIN_DIR) : 
	mkdir $(OUTPUT_BIN_DIR)

clean :
	rm -rf $(OUTPUT_BIN_DIR)

check : $(OUTPUT_BIN_DIR)/$(TARGET)
	./$(OUTPUT_BIN_DIR)/$(TARGET)

