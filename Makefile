define doinsubdirs
$(foreach d,$(1),$(MAKE) -C $(d) $(2) $@;)
endef

SUBDIRS = core utils

.PHONY: all check clean googletest test

all check clean:
	$(call doinsubdirs,${SUBDIRS})

googletest:
	$(MAKE) -C googletest/make
