ifneq ($(V),1)
Q = @
MAKEFLAGS += --no-print-dir
endif

.PHONY: all
all:
	$(Q)if [ ! -f libopencm3/Makefile ]; then \
		echo "Initialising git submodules..." ;\
		git submodule init ;\
		git submodule update ;\
	fi
	$(Q)$(MAKE) -C libopencm3 lib
	$(Q)$(MAKE) -C src

.PHONY: clean
clean:
	$(Q)$(MAKE) -C libopencm3 $@
	$(Q)$(MAKE) -C src $@
