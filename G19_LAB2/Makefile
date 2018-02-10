BINDIR=MDK-ARM/G19_LAB2
GDBDIR=MDK-ARM

flash:
	st-flash write $(BINDIR)/G19_LAB2.bin 0x08000000

deploy:
	st-util &
	cd $(GDBDIR); arm-none-eabi-gdb G19_LAB2/G19_LAB2.axf -ex "tar extended-remote :4242"
	pkill st-util
