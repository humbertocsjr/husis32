; Extensao das rotinas de acesso ao terminal do Sistema inicial
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.28 - Versao inicial

TermEscrevaDS:
    jmp termext1
TermEscrevaC:
    jmp termext2

termext1:
    push si
    push ax
    termext1Repete:
        lodsb
        cmpb al, #0
        je termext1Fim
        cmpb al, #'|'
        je termext1Fim
        movb ah, #0xe
        int #0x10
        jmp termext1Repete
    termext1Fim:
    pop ax
    pop si
    ret

termext2:
    push ax
    movb ah, #0xe
    int #0x10
    pop ax
    ret
