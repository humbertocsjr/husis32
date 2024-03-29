all:
	@sh gerar_versao.sh
	@cd HUSIS/Nucleo.prg && make
	@rm -Rf Temp/*
	@find boot -name \*.cfg -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -name \*.rot -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -name \*.bin -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -name \*.elf -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -name \*.cfg -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -name \*.md -exec install -D '{}' Temp'/{}' ';'
	@find . -name license.txt -exec install -D '{}' Temp'/{}' ';'
	@find HUSIS -wholename \*.cf/\*.\* -exec install -D '{}' Temp'/{}' ';'
	@tree Temp/
	@grub-mkrescue -o Imagens/Disco.iso Temp
	@dd if=/dev/zero of=Imagens/Disquete.img bs=1024 count=1340
	@minixfs mkfs Imagens/Disquete.img -1 -n 30 -s 1340
	@dd if=boot/stage1 of=Imagens/Disquete.img conv=notrunc
	@cat boot/stage2 >> Imagens/Disquete.img
	@minixfs mkdir Imagens/Disquete.img boot
	@minixfs mkdir Imagens/Disquete.img boot/grub
	@minixfs add Imagens/Disquete.img boot/grub/menu.lst boot/grub/menu.lst
	@minixfs mkdir Imagens/Disquete.img HUSIS
	@minixfs mkdir Imagens/Disquete.img HUSIS/Nucleo.prg
	@minixfs add Imagens/Disquete.img HUSIS/Nucleo.prg/prog.elf HUSIS/Nucleo.prg/prog.elf
	@minixfs add Imagens/Disquete.img HUSIS/Nucleo.prg/prog.rot HUSIS/Nucleo.prg/prog.rot
	@minixfs add Imagens/Disquete.img HUSIS/Nucleo.prg/prog.bin HUSIS/Nucleo.prg/prog.bin
	@rm -Rf Temp/*

bochs: all
	@bochs
qemu: all
	@qemu-system-i386 -rtc base=localtime -m 32 Imagens/Disco.iso

disquete: all
	@qemu-system-i386 -rtc base=localtime -m 32 -fda Imagens/Disquete.img -no-reboot 
	
86box: all
	@cd ../86Box && ./86Box.AppImage
