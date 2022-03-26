ARQUIVOS =  es_386pc.c es_386pc_int.c nucleo.c iut.c sisarq.c mem.c processo.c dispositivo.c txt.c disco.c disco/disquete.c disco/ide.c es/teclado.c es/interface.c es/relogio.c es.c agenda.c

all: ram64

ram64:
	cp config64mib.h config.h
	make nucleo
ram128:
	cp config128mib.h config.h
	make nucleo
ram256:
	cp config256mib.h config.h
	make nucleo
ram4096:
	cp config4gib.h config.h
	make nucleo

nucleo:	
	smlrcc -asm n2f -aout -o prog.rot $(ARQUIVOS)
	smlrcc -asm n2f -flat32 -origin 0x100000 -o prog.bin $(ARQUIVOS)
	smlrcc -asm n2f -elf -origin 0x100000 -o prog.elf $(ARQUIVOS)
