; Interface com o Usuario
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.03.07 - Prototipo
;
.include /HUSIS/Base.cf/baseprg.s

Pilha = 4096

Versao = 22
SubVersao = 03
Revisao= 07 * 10 + SubRevisao
SubRevisao = 0

Exportar:
    .word FimExportar
Importar:
    .include /HUSIS/Base.cf/husis.s
    .include /HUSIS/Base.cf/txt.s
    .word FimImportar

Inicial:
    retf


Fim:
