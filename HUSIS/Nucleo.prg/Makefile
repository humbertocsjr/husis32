all:
	smlrcc -asm n2f -aout -o prog.rot es_386pc.c nucleo.c iut.c sisarq.c
	smlrcc -asm n2f -elf -origin 0x100000 -o prog.elf es_386pc.c nucleo.c iut.c sisarq.c
