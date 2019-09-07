
AS := gpasm
RM := rm -f

PKFLAGS := -W -M
#PKFLAGS := -X -M

default: ggtrack.hex


flash: ggtrack.hex
	pk2cmd $(PKFLAGS) -PPIC16F628A -F$<

.PHONY: flash


clean:
	$(RM) *.cod *.hex *.o *.lst

.PHONY: clean


%.hex: %.asm
	$(AS) -o $@ $<

#   -DCONFIG_WORD=_INTOSC_OSC_NOCLKOUT\\\&_WDT_OFF\\\&_LVP_OFF\\\&_DATA_CP_OFF\\\&_PWRTE_ON \

