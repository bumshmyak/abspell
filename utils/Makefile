SRCROOT = $(realpath ../)
include $(SRCROOT)/config.mak

.PHONY: clean all 

all: stats dict-search best_word_corrector
	cd ../core; make all; cd -

stats : stats.cc
	$(CXX) -g -o $@ $<

dict-search: dict-search.cc
	$(CXX) -g -o $@ $< -I$(INCLUDES_PATH) -I$(BOOST_INCLUDE_PATH)

best_word_corrector: best_word_corrector.cc
	$(CXX) -o $@ $< $(CXXFLAGS) -I$(INCLUDES_PATH) -labspell -L$(INSTALL_LIB_PATH)

clean:
	rm -f stats dict-search best_word_corrector
