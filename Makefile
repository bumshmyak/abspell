define doinsubdirs
$(foreach d,$(1),$(MAKE) -C $(d) $(2) $@;)
endef

SUBDIRS = core test

.PHONY: all check clean googletest test

all check clean:
	$(call doinsubdirs,${SUBDIRS})

test:
	-cd test; $(MAKE) test; cd -

