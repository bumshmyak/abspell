# default settings for all subprojects

INCLUDES_PATH = $(SRCROOT)/include
BOOST_INCLUDE_PATH=${SRCROOT}/boost
GTEST_INCLUDE_PATH = $(SRCROOT)/googletest/include

INSTALL_LIB_PATH = $(SRCROOT)/lib
GTEST_LIB_PATH = $(SRCROOT)/googletest/make

CXXFLAGS = -O3 -g -Wall -Wno-sign-compare -Werror -pthread -I$(INCLUDES_PATH) -I${BOOST_INCLUDE_PATH}

OUTPUT_BIN_DIR = bin
