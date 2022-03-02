; Rotinas de acesso ao disco do Sistema Inicial
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao inicial

DiscoLeia:
    jmp disco1
DiscoLeiaBloco:
    jmp disco2


disco1:
    push ax
    push bx
    push ds
    push cs
    pop ds
    shl ax
    call DiscoLeiaBloco
    inc ax
    add bx, #512
    call DiscoLeiaBloco
    pop ds
    pop bx
    pop ax
    ret

disco2:
    push ax
    push bx
    push cx
    push dx
    call TermEscrevaPonto
    xor dx, dx
    div ParamSetores
    movb cl, dl
    incb cl
    xor dx, dx
    div ParamCabecas
    movb ch, al
    movb dh, dl
    seg cs
    movb dl, GlobalDisco
    movb al, #1
    movb ah, #2
    disco2Tentativa:
    push ax
    push bx
    push cx
    push dx
    int #0x13
    jnc disco2Fim
        xor ax, ax
        movb dl, GlobalDisco
        int #0x13
        call TermEscrevaPonto
        pop dx
        pop cx
        pop bx
        pop ax
        jmp disco2Tentativa
    disco2Fim:
    pop dx
    pop cx
    pop bx
    pop ax
    pop dx
    pop cx
    pop bx
    pop ax
    ret
