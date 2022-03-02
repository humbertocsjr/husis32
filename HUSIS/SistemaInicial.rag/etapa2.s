; Etapa 2 do Sistema Inicial
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.28 - Versao inicial

inicio:
;etapa21:
    TradEtapa2 = 0
    TradOk = 1
    TradArqConf = 2
    TradOpcoes = 3
    TradEscolha = 4
    TradAuto = 5
    TradManual = 6
    TradIniciando = 7
    TradEncerrado = 8

    mov InicialEscrevaNro, dx
    mov InicialEscrevaDS, cx
    mov InicialCarregaArquivo, bx
    mov InicialLeiaBloco, ax
    mov InicialEscrevaC, di
    mov ax, es
    mov InicialEscrevaNro + 2, ax
    mov InicialEscrevaDS + 2, ax
    mov InicialCarregaArquivo + 2, ax
    mov InicialLeiaBloco + 2, ax
    mov InicialEscrevaC + 2, ax
    push cs
    pop es

    call termEscrevaEnter
    mov ax, #TradEtapa2
    call tradEscreva

    call termEscreva
    .asciz " ["
    call calculaFim
    call termEscrevaNro
    call termEscreva
    .asciz " Bytes ]"
    call termEscrevaEnter
    call termEscreva
    .ascii " Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)"
    .byte 13
    .byte 10
    .ascii " http://humbertocsjr.dev.br/"
    .byte 13
    .byte 10
    .byte 10
    .ascii " "
    .byte 0

    call calculaFimSeg
    mov es, ax
    mov GlobalConf, ax

    mov ax, #TradArqConf
    call tradCalculaSI
    call arqCarrega

    call termEscrevaEnter
    call termEscrevaEnter
    mov ax, #TradOpcoes
    call tradEscreva
    call termEscrevaEnter
    call termEscrevaEnter

    xor si, si
    mov cx, #ConstQtdOpcoes
    etapa21Escreve:
        call pulaEspacos
        seg es
        lodsb
        cmpb al, #'*'
        je etapa21EscreveFim
        cmpb al, #'0'
        jb etapa21EscreveFim
        cmpb al, #'9'
        ja etapa21EscreveFim
        call termEscreva
        .asciz " "
        call termEscrevaC
        call termEscreva
        .asciz ") "
        call pulaIgual
        call termEscrevaNomeES
        call termEscrevaEnter
        call proximoRegistro
        loop etapa21Escreve
    etapa21EscreveFim:

    call termEscrevaEnter
    mov ax, #TradEscolha
    call tradEscreva
    call termEscreva
    .ascii " ."
    .byte 8
    .byte 0
    xor ax, ax
    int #0x1a
    mov cx, #5
    etapa21Espera:
        push cx
        mov GlobalTicks, dx
        add dx, #18
        mov GlobalTicksProx, dx
        call termEscreva
        .byte 8
        .byte 0
        pop cx
        push cx
        movb al, cl
        addb al, #'0'
        call termEscrevaC
        etapa21Segundo:
            movb ah, #1
            int #0x16
            jz etapa21SemTecla
                movb ah, #0
                int #0x16
                push si
                push cx
                xor si, si
                etapa21BuscaTecla:
                    call pulaEspacos
                    seg es
                    cmpb al, [si]
                    jne etapa21ContinuaTecla
                        pop cx
                        pop cx          ; Descarte
                        jmp etapa21EscolhaManual
                    etapa21ContinuaTecla:
                    call proximoRegistro
                    loop etapa21BuscaTecla
                pop cx
                pop si
            etapa21SemTecla:
            xor ax, ax
            int #0x1a
            cmp dx, GlobalTicks
            jb etapa21SegundoFim
            cmp dx, GlobalTicksProx
            jb etapa21Segundo
        etapa21SegundoFim:
        pop cx
        loop etapa21Espera

    call termEscreva
    .byte 8
    .byte 0

    mov ax, #TradAuto
    call tradEscreva
    xor si, si
    call pulaEspacos
    jmp etapa21Escolhido
    etapa21EscolhaManual:
        call termEscreva
        .byte 8
        .byte 0
        push ax
        mov ax, #TradManual
        call tradEscreva
        pop ax
        call termEscrevaC
    etapa21Escolhido:
    call termEscrevaEnter
    call pulaIgual
    mov ax, #TradIniciando
    call tradEscreva
    call termEscrevaNomeES
    call termEscrevaEnter
    call pulaCampo
    mov ax, es
    mov ds, ax
    mov ax, #0x710
    mov es, ax
    call arqCarrega
    call termEscreva
    .asciz ">"
    mov ax, cs
    mov es, ax
    mov ax, #0x700
    mov ds, ax
    call termEscrevaNro
    call termEscreva
    .asciz ":"
    push cs
    mov ax, #infinito
    push ax
    mov ax, 0x104
    call termEscrevaNro
    call termEscrevaEnter
    call termEscrevaEnter
    push ds
    push ax
    seg cs
    mov ax, InicialCarregaArquivo+2
    mov es, ax
    seg cs
    mov ax, InicialLeiaBloco
    seg cs
    mov bx, InicialCarregaArquivo
    seg cs
    mov cx, InicialEscrevaDS
    seg cs
    mov dx, InicialEscrevaNro
    seg cs
    mov di, InicialEscrevaC
    push cs
    pop si
    retf






infinito:
    push cs
    pop ds
    call termEscrevaEnter
    mov ax, #TradEncerrado
    call tradEscreva
    infinito1:
        hlt
        jmps infinito1


calculaFim:
;etapa22:
    mov ax, #Fim
    add ax, Fim + 8
    ret

calculaFimSeg:
;etapa23:
    push bx
    call calculaFim
    push ax
    mov ax, cs
    pop bx
    shr bx
    shr bx
    shr bx
    shr bx
    inc bx
    add ax, bx
    pop bx
    ret

pulaEspacos:
;etapa24:
    push ax
    etapa24Inicio:
        seg es
        lodsb
        cmpb al, #13
        je etapa24Inicio
        cmpb al, #10
        je etapa24Inicio
        cmpb al, #' '
        je etapa24Inicio
        cmpb al, #9
        je etapa24Inicio
    dec si
    pop ax
    ret

proximoRegistro:
;etapa25:
    push ax
    etapa25Inicio:
        seg es
        lodsb
        cmpb al, #13
        je etapa25Fim
        cmpb al, #10
        je etapa25Fim
        cmpb al, #'*'
        je etapa25Fim
        jmp etapa25Inicio
    etapa25Fim:
    dec si
    pop ax
    ret

pulaIgual:
;etapa26:
    push ax
    etapa26Inicio:
        seg es
        lodsb
        cmpb al, #'='
        je etapa26Fim
        cmpb al, #'*'
        je etapa26FimArq
        jmp etapa26Inicio
        etapa26FimArq:
        dec si
    etapa26Fim:
    pop ax
    ret

pulaCampo:
;etapa27:
    push ax
    etapa27Inicio:
        seg es
        lodsb
        cmpb al, #'|'
        je etapa27Fim
        cmpb al, #10
        je etapa27FimArq
        cmpb al, #13
        je etapa27FimArq
        cmpb al, #'*'
        je etapa27FimArq
        jmp etapa27Inicio
        etapa27FimArq:
        dec si
    etapa27Fim:
    pop ax
    ret


termEscrevaNro:
;term1:
    seg cs
    callf InicialEscrevaNro
    ret

termEscrevaDS:
;term2:
    seg cs
    callf InicialEscrevaDS
    ret

termEscreva:
;term3:
    push bp
    mov bp, sp
    push ds
    push ax
    push si
    mov si, 2[bp]
    push cs
    pop ds
    term3Repete:
        lodsb
        cmpb al, #0
        je term3Fim
        seg cs
        callf InicialEscrevaC
        jmp term3Repete
    term3Fim:
    mov 2[bp], si
    pop si
    pop ax
    pop ds
    mov sp, bp
    pop bp
    ret

termEscrevaPonto:
;term4
    call termEscreva
    .asciz "."
    ret

termEscrevaOk:
;term4
    push ax
    mov ax, #TradOk
    call tradEscreva
    pop ax
    ret

termEscrevaEnter:
;term5
    call termEscreva
    .byte 13
    .byte 10
    .byte 0
    ret

termEscrevaNomeES:
;term6:
    push ax
    push si
    term6Repete:
        seg es
        lodsb
        cmpb al, #0
        je term6Fim
        cmpb al, #13
        je term6Fim
        cmpb al, #10
        je term6Fim
        cmpb al, #'|'
        je term6Fim
        cmpb al, #'*'
        je term6Fim
        seg cs
        callf InicialEscrevaC
        jmp term6Repete
    term6Fim:
    pop si
    pop ax
    ret

termEscrevaC:
;term7
    seg cs
    callf InicialEscrevaC
    ret

tradCalculaSI:
;trad1:
    push ds
    push ax
    mov si, ax
    shl si
    add si, #10 + Fim
    mov ax, [si]
    add ax, #Fim
    mov si, ax
    pop ax
    pop ds
    ret

tradEscreva:
;trad2:
    push si
    call tradCalculaSI
    call termEscrevaDS
    pop si
    ret

arqCarrega:
    seg cs
    callf InicialCarregaArquivo
    ret

GlobalConf:
    .word 0
ConstQtdOpcoes = 10
GlobalTicks:
    .word 0
GlobalTicksProx:
    .word 0

InicialEscrevaNro:
    .word 0
    .word 0
InicialEscrevaDS:
    .word 0
    .word 0
InicialEscrevaC:
    .word 0
    .word 0
InicialLeiaBloco:
    .word 0
    .word 0
InicialCarregaArquivo:
    .word 0
    .word 0

Fim:

