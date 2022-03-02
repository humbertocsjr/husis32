
memIniciar:
    push ax
    push bx
    push cx
    push dx
    push es
    push di
    ; Buscando tamanho da memoria em KiB
    xor ax, ax
    int #0x12
    ; Calculando em Segmentos
    mov bx, #1024/16
    mul bx
    mov GlobalMemTam, ax
    ; Calculando em Blocos
    mov bx, #16
    div bx
    mov GlobalMemBlocos, ax
    ; Definindo Mapa da Memoria
    mov ax, cs
    add ax, #FimSeg
    mov GlobalMemPos, ax
    mov es, ax
    ; Zerando mapa
    xor ax, ax
    mov cx, GlobalMemBlocos
    xor di, di
    rep
    stosw
    ; Calculando o tamanho do mapa em blocos
    mov ax, di
    movb al, ah
    xorb ah, ah
    push ax
    ; Calculando a posicao do mapa em blocos
    mov ax, es
    shr ax
    shr ax
    shr ax
    shr ax
    ; Somando para descobrir o inicio da memoria livre
    pop bx
    add ax, bx
    inc ax
    ; Marcando memoria como reservada
    mov cx, ax
    mov ax, #0xffff
    xor di, di
    rep
    stosw
    ; Fim da inicializacao da memoria
    pop di
    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    ret

; MemLivre256
; Retorna quantidade de blocos livres
; ret: ax: Qtd de blocos livres
mem1:
    push cx
    push bx
    push si
    push es
    seg cs
    mov ax, GlobalMemPos
    mov es, ax
    seg cs
    mov cx, GlobalMemBlocos
    xor bx, bx
    xor si, si
    mem1Busca:
        seg es
        lodsw
        cmp ax, #0
        jne mem1Continua
            inc bx
        mem1Continua:
        loop mem1Busca
    mov ax, bx
    pop es
    pop si
    pop bx
    pop cx
    retf

; MemAlocar256
; Aloca um ou mais blocos de 256 bytes
; cx: Qtd de blocos
; ret: cf: 1=Ok | 0=Estouro de memoria
;      es: Segmento alocado, caso nao encontre sera definido como ds
mem2:
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov dx, cx
    seg cs
    mov ax, GlobalMemPos
    mov es, ax
    seg cs
    mov cx, GlobalMemBlocos
    xor bx, bx
    xor di, di
    xor si, si
    mem2Busca:
        seg es
        lodsw
        cmp ax, #0
        jne mem2ZeraEncontrados
            cmp bx, #0
            je mem2IniciaVerificacao
                inc di
                cmp di, dx
                jb mem2ContinuaBuscando
                jmp mem2Encontrado
            mem2IniciaVerificacao:
            mov bx, si
            dec bx
            mov di, #1
            jmps mem2ContinuaBuscando
        mem2ZeraEncontrados:
        xor bx, bx
        xor di, di
        mem2ContinuaBuscando:
        loop mem2Busca
    push ds
    pop es
    clc
    jmps mem2Fim
    mem2Encontrado:
    mov di, bx
    mov cx, dx
    seg cs
    movb al, GlobalProcessoAtual
    xorb ah, ah
    mem2IniciaConjunto:
        stosw
        incb ah
        loop mem2IniciaConjunto
    shl bx
    shl bx
    shl bx
    shl bx
    push bx
    pop es
    stc
    mem2Fim:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    retf

; MemLiberar256
; Libera um conjunto de blocos de 256 bytes
; es: Primeiro segmento do conjunto a ser liberado
mem3:
    retf
