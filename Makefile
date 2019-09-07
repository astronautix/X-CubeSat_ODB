
ARCH   ?= stm32f4
TOPDIR ?= $(CURDIR)

default: ODB ADCS

doc:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) doc

check:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) check

clean:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) clean

tags:
	find . -type f \( -name "*.[ch]" -o -name "*.cpp" \) -print0 | xargs -0 ctags

ODB ADCS:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) $@

flashODB flashADCS:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) $@

cleanODB cleanADCS:
	@$(MAKE) -C $(TOPDIR)/sat TOPDIR=$(TOPDIR) $@

.PHONY: default doc check clean tags
.PHONY: ODB ADCS
.PHONY: flashODB flashADCS
.PHONY: cleanODB cleanADCS
