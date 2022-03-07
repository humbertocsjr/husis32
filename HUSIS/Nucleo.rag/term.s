; Modulo de Terminal
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao inicial

; TermEscreverC
; Escreve um caractere na tela
; al: Caractere
term1:
    push ax
    push bx
    push bp
    movb ah, #0xe
    mov bx, #0xf
    int #0x10
    pop bp
    pop bx
    pop ax
    retf

; TermEscrever
; Escreve um texto diretamente apos a CALL
; cs:ip: Texto
; cx: Tamanho (0=Dinamico)
term2:
    push bp
    mov bp, sp
    push ds
    push si
    push ax
    push cx
    mov si, 2[bp]
    mov ax, 4[bp]
    mov ds, ax
    cmp cx, #0
    jne term2Escreve
        mov cx, #1024
    term2Escreve:
        lodsb
        cmpb al, #0
        je term2Fim
        seg cs
        callf TermEscreverC
        loop term2Escreve
    term2Fim:
    mov 2[bp], si
    pop cx
    pop ax
    pop si
    pop ds
    mov sp, bp
    pop bp
    retf

; TermEscreverEnter
; Cria uma nova linha
term3:
    push ax
    movb al, #13
    seg cs
    callf TermEscreverC
    movb al, #10
    seg cs
    callf TermEscreverC
    pop ax
    retf

; TermEscreverL
; Escreve um texto diretamente apos a CALL, criando uma nova linha
; cs:ip: Texto
; cx: Tamanho (0=Dinamico)
term4:
    push bp
    mov bp, sp
    push ds
    push si
    push ax
    push cx
    mov si, 2[bp]
    mov ax, 4[bp]
    mov ds, ax
    cmp cx, #0
    jne term4Escreve
        mov cx, #1024
    term4Escreve:
        lodsb
        cmpb al, #0
        je term4Fim
        seg cs
        callf TermEscreverC
        loop term4Escreve
    term4Fim:
    mov 2[bp], si
    seg cs
    callf TermEscreverEnter
    pop cx
    pop ax
    pop si
    pop ds
    mov sp, bp
    pop bp
    retf

; TermEscreverNro e TermEscreverNroEspaco
; ax: Numero
term5:
    seg cs
    callf TermEscrever
    .asciz " "
    term5Inicio:
    push ax
    push bx
    push dx
    xor dx, dx
    cmp ax, #0
    je term5Fim
        mov bx, #10
        div bx
        cmp ax, #0
        je term5Fim
        push cs
        call term5Inicio
    term5Fim:
    mov ax, dx
    addb al, #'0'
    seg cs
    callf TermEscreverC
    pop dx
    pop bx
    pop ax
    retf
