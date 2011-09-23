# default settings for all subprojects

INCLUDES_PATH = $(SRCROOT)/include

INSTALL_LIB_PATH = $(SRCROOT)/lib

CXXFLAGS = -g -Wall -Wno-sign-compare -Werror -pthread -I$(INCLUDES_PATH)

OUTPUT_BIN_DIR = bin