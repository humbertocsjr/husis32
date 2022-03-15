all:
	smlrcc -asm n2f -flat32 -o prog.bin es_386pc.c nucleo.c iut.c
	smlrcc -asm n2f -elf -origin 0x100000 -o prog.rot es_386pc.c nucleo.c iut.c
