define doinsubdirs
$(foreach d,$(1),$(MAKE) -C $(d) $(2) $@;)
endef

SUBDIRS = core

.PHONY: all check clean googletest

all check clean:
	$(call doinsubdirs,${SUBDIRS})
