# Eliminating evil software piece by piece
BINDIR=MDK-ARM/LAB4
GDBDIR=MDK-ARM
KEIL_TOOLS=/home/harwiltz/.wine/drive_c/Keil_v5/ARM/ARMCC/bin

compile:
	wine $(KEIL_TOOLS)/fromelf.exe --bin $(BINDIR)/LAB4.axf --output $(BINDIR)/G19_LAB4.bin

flash: compile
	st-flash write $(BINDIR)/G19_LAB4.bin 0x08000000

deploy: flash
	st-util &
	cd $(GDBDIR); arm-none-eabi-gdb LAB4/LAB4.axf -ex "tar extended-remote :4242"
	pkill st-util
