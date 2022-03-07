; Modulo de Manipulacao de Texto
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao inicial 

; TxtTamanhoDSDS
; Conta o tamanho de uma string
; ret: cx: Tamanho
txt1:
    pushf
    push ax
    push si
    xor cx, cx
    txt1Repeticao:
        lodsb
        cmpb al, #0
        je txt1Fim
        inc cx
        jmp txt1Repeticao
    txt1Fim:
    pop si
    pop ax
    popf
    retf

; TxtCopiarDSES
; Copia um texto
; ds:si = Origem
; es:di = Destino
; cx = Tamanho (0 = Dinamico)
txt2:
    pushf
    push ax
    push cx
    push si
    push di
    cld
    cmp cx, #0
    jne txt2Copiar
        seg cs
        callf TxtTamanhoDS
    txt2Copiar:
        lodsb
        stosb
        cmpb al, #0
        je txt2Fim
        loop txt2Copiar
    txt2Fim:
    pop di
    pop si
    pop cx
    pop ax
    popf
    retf

; TxtCopiarDSDS
; Copia um texto
; ds:si = Origem
; ds:di = Destino
; cx = Tamanho (0 = Dinamico)
txt3:
    push es
    push ds
    pop es
    seg cs
    callf TxtCopiarDSES
    pop es
    retf

; TxtCopiarESDS
; Copia um texto
; es:si = Origem
; ds:di = Destino
; cx = Tamanho (0 = Dinamico)
txt4:
    push es
    push ds
    push es
    push ds
    pop es
    pop ds
    seg cs
    callf TxtCopiarDSES
    pop ds
    pop es
    retf

; TxtCopiarCSDS
; Copia um texto
; cs:si = Origem
; ds:di = Destino
; cx = Tamanho (0 = Dinamico)
txt5:
    push es
    push ds
    push cs
    push ds
    pop es
    pop ds
    seg cs
    callf TxtCopiarDSES
    pop ds
    pop es
    retf

; TxtCopiarCSES
; Copia um texto
; cs:si = Origem
; es:di = Destino
; cx = Tamanho (0 = Dinamico)
txt6:
    push ds
    push cs
    pop ds
    seg cs
    callf TxtCopiarDSES
    pop ds
    retf


; TxtConcatDSES
; Concatena um texto
; ds:si = Origem
; es:di = Destino
; cx = Tamanho do destino
; ret: cf = 1=Ok | 0=Estouro
txt7:
    push ax
    push cx
    push si
    push di
    pushf
    cld
    cmp cx, #0
    jne txt7PulaInicio
    jmp txt7FimErro
    txt7PulaInicio:
        cmpb [di], #0
        je txt7Copiar
        inc di
        loop txt7PulaInicio
    txt7Copiar:
        lodsb
        stosb
        cmpb al, #0
        je txt7FimOk
        loop txt7Copiar
    dec di
    seg es
    movb [di], #0
    txt7FimErro:
    popf
    clc
    jmps txt7Fim
    txt7FimOk:
    popf
    stc
    txt7Fim:
    pop di
    pop si
    pop cx
    pop ax
    retf

; TxtConcatDSDS
; Concatena um texto
; ds:si = Origem
; ds:di = Destino
; cx = Tamanho do destino
txt8:
    push es
    push ds
    pop es
    seg cs
    callf TxtConcatDSES
    pop es
    retf

; TxtConcatESDS
; Concatena um texto
; es:si = Origem
; ds:di = Destino
; cx = Tamanho do destino
txt9:
    push es
    push ds
    push es
    push ds
    pop es
    pop ds
    seg cs
    callf TxtConcatDSES
    pop ds
    pop es
    retf

; TxtCopiarCSDS
; Copia um texto
; cs:si = Origem
; ds:di = Destino
; cx = Tamanho do destino
txt10:
    push es
    push ds
    push cs
    push ds
    pop es
    pop ds
    seg cs
    callf TxtConcatDSES
    pop ds
    pop es
    retf

; TxtConcatCSES
; Copia um texto
; cs:si = Origem
; es:di = Destino
; cx = Tamanho do destino
txt11:
    push ds
    push cs
    pop ds
    seg cs
    callf TxtConcatDSES
    pop ds
    retf
