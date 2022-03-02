; Rotinas de acesso ao terminal do Sistema Inicial
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao Inicial

TermEscreva:
    jmp term1
TermEscrevaNro:
    jmp term2Inicio
TermEscrevaNroEspaco:
    jmp term2
TermEscrevaPonto:
    jmp term3
TermEscrevaOk:
    jmp term4

term1:
    push bp
    mov bp, sp
    push si
    push ax
    mov si, 2[bp]
    term1Repeticao:
        seg cs
        lodsb
        cmpb al, #0
        je term1Fim
        movb ah, #0xe
        int #0x10
        jmp term1Repeticao
    term1Fim:
    mov 2[bp], si
    pop ax
    pop si
    mov sp, bp
    pop bp
    ret

term2:
    call TermEscreva
    .asciz " "
    term2Inicio:
    push ax
    push bx
    push dx
    xor dx, dx
    cmp ax, #0
    je term2Fim
        mov bx, #10
        div bx
        cmp ax, #0
        je term2Fim
        call term2Inicio
    term2Fim:
    mov ax, dx
    addb al,#'0'
    movb ah, #0xe
    int #0x10
    pop dx
    pop bx
    pop ax
    ret

term3:
    call TermEscreva
    .asciz "."
    ret

term4:
    call TermEscreva
    .ascii "[ OK ]"
    .byte 13
    .byte 10
    .byte 0
    ret
