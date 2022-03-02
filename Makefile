all:
	@make --no-print-directory img360
	@make --no-print-directory img1440
	@echo == Resultado
	@sudo mount  img360.img Temp/
	@tree Temp
	@df Temp
	@sudo umount Temp
teste:
	@! dosemu -dumb -d ./ -E "G:\DOSEMU.BAT"
img360:
	@echo == Criando imagem de 360 KiB
	@dd if=/dev/zero of=imagem.img bs=1024 count=360 status=none
	@mkfs.minix -1 -n30 imagem.img 360 > /dev/null
	@dd if=HUSIS/SistemaInicial.rag/inicial.bin of=imagem.img conv=notrunc status=none
	@perl -e 'print pack "c*", 9,0,2,0,40,0' | dd of=imagem.img bs=1 seek=492 count=6 status=none conv=notrunc
	@make --no-print-directory arrumaimagem
	@mv imagem.img img360.img
img1440:
	@echo == Criando imagem de 1440 KiB
	@dd if=/dev/zero of=imagem.img bs=1024 count=1440 status=none
	@mkfs.minix -1 -n30 imagem.img 1440 > /dev/null
	@dd if=HUSIS/SistemaInicial.rag/inicial.bin of=imagem.img conv=notrunc status=none
	@perl -e 'print pack "c*", 18,0,2,0,80,0' | dd of=imagem.img bs=1 seek=492 count=6 status=none conv=notrunc
	@make --no-print-directory arrumaimagem
	@mv imagem.img img1440.img
arrumaimagem:
	@rm -fR ./Temp
	@mkdir Temp
	@sudo mount  imagem.img Temp/
	@sudo chmod -R +rw Temp/
	@echo == Copiando Sistema Operacional
	@cp HUSIS/SistemaInicial.rag/etapa2.bin Etapa2.bin
	@cat HUSIS/SistemaInicial.rag/ptbr.bin >> Etapa2.bin
	@cp Etapa2.bin Temp/Etapa2.bin
	@cp license Temp/License.txt
	@mkdir Temp/HUSIS
	@mkdir Temp/Programas
	@mkdir Temp/Perfis
	@mkdir Temp/Desenvolvimento
	@find HUSIS -type f '(' -wholename '*.cf/*.s' -o -name ptbr.s -o -name \*.bas -o -name \*.ba8 -o -name \*.b16 -o -name \*.b32 -o -name \*.rot -o -name \*.bin -o -name \*.cfg -o -name \*.md ')' -exec install -D '{}' Temp/'{}' ';'
	@find Programas -type f '(' -wholename '*.cf/*.s' -o -name ptbr.s -o -name \*.bas -o -name \*.ba8 -o -name \*.b16 -o -name \*.b32 -o -name \*.rot -o -name \*.bin -o -name \*.cfg -o -name \*.md ')' -exec install -D '{}' Temp/'{}' ';'
	@find Desenvolvimento -type f '(' -wholename '*.cf/*.s' -o -name ptbr.s -o -name \*.bas -o -name \*.ba8 -o -name \*.b16 -o -name \*.b32 -o -name \*.rot -o -name \*.bin -o -name \*.cfg -o -name \*.md ')' -exec install -D '{}' Temp/'{}' ';'
	@sudo umount Temp
