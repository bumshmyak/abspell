SRCROOT = $(realpath ../)
include $(SRCROOT)/config.mak

LDFLAGS += -L$(INSTALL_LIB_PATH) -labspell
LDFLAGS += -Wl,-rpath=$(INSTALL_LIB_PATH)

HEADERS = $(INCLUDES_PATH)/*.h

.PHONY: check clean all 

all: stats dict-search best_word_corrector test_metaphone phrase_corrector

stats : stats.cc
	$(CXX) -g -o $@ $<

dict-search: dict-search.cc
	$(CXX) -g -o $@ $< -I$(INCLUDES_PATH) -I$(BOOST_INCLUDE_PATH) $(LDFLAGS) $(CXXFLAGS)

best_word_corrector: best_word_corrector.cc $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) -I$(INCLUDES_PATH) $(LDFLAGS) $(CXXFLAGS)

test_metaphone: test_metaphone.cc $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) -I$(INCLUDES_PATH) $(LDFLAGS) $(CXXFLAGS)

phrase_corrector: phrase_corrector.cc $(HEADERS)
	$(CXX) -o $@ $< $(CXXFLAGS) -I$(INCLUDES_PATH) $(LDFLAGS) $(CXXFLAGS)

clean:
	rm -f stats dict-search best_word_corrector

check:

