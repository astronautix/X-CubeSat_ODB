
ARCH   ?= stm32f4
SATDIR ?= $(TOPDIR)/sat

default: ODB ADCS

ODB ADCS: sys
	@$(MAKE) -C $(SATDIR)/$@ SATDIR=$(SATDIR)

sys:
	@$(MAKE) -C $(SATDIR)/$@ SATDIR=$(SATDIR) ARCH=$(ARCH)

check:
	$(CHECK) -I. -I$(SATDIR)/sys -I$(SATDIR)/sys/device -I$(SATDIR)/sys/system ./sys/system ./sys/device ./ODB

clean: cleanODB cleanADCS

flashODB: ODB
	@$(MAKE) -C $(SATDIR)/ODB flash

flashADCS: ADCS
	@$(MAKE) -C $(SATDIR)/ADCS flash

cleanODB: cleanSys
	@$(MAKE) -C $(SATDIR)/ODB SATDIR=$(SATDIR) clean

cleanADCS: cleanSys
	@$(MAKE) -C $(SATDIR)/ADCS SATDIR=$(SATDIR) clean

cleanSys:
	@$(MAKE) -C $(SATDIR)/sys SATDIR=$(SATDIR) ARCH=$(ARCH) clean

doc:
	doxygen $(SATDIR)/rules/Doxyfile


.PHONY: default check clean doc
.PHONY: sys ODB ADCS
.PHONY: flashODB flashADCS
.PHONY: cleanODB cleanADCS

include $(SATDIR)/rules/common.mk
