; Nucleo do HUSIS
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao inicial
; - 22.03.07 - Integrado o modulo de unidades de disco
;
; Versionamento do HUSIS:
; - Versao sempre sera o Ano atual
; - SubVersao sempre sera o Mes atual
; - Revisao sempre sera o Dia atual multiplicado por dez, somado a subrevisao
;   do dia
;
;   Exemplo: Data: 2022.03.07 SubRevisao 3 -> v22.03 R73 
;
;   O versionamento do Sistema Operacional HUSIS eh baseado no versionamento
;   do nucleo, que eh definido neste arquivo.
;

.include /HUSIS/Base.cf/baseprg.s

Versao = 22                             ; Ano
SubVersao = 03                          ; Mes
Revisao = 07        * 10 + SubRevisao   ; Dia * 10 + SubRevisao dentro do dia
SubRevisao = 0                          ; Deve ser zerado a cada dia

Pilha = 4096

Exportar:
    ArrumarExportar:                    ; 0
        .word prog1
        .word 0
    TxtTamanhoDS:                       ; 1
        .word txt1
        .word 0
    TxtCopiarDSES:                      ; 2
        .word txt2
        .word 0
    TxtCopiarDSDS:                      ; 3
        .word txt3
        .word 0
    TxtCopiarESDS:                      ; 4
        .word txt4
        .word 0
    TxtCopiarCSDS:                      ; 5
        .word txt5
        .word 0
    TxtCopiarCSES:                      ; 6
        .word txt6
        .word 0
    TxtConcatDSES:                      ; 7
        .word txt7
        .word 0
    TxtConcatDSDS:                      ; 8
        .word txt8
        .word 0
    TxtConcatESDS:                      ; 9
        .word txt9
        .word 0
    TxtConcatCSDS:                      ; 10
        .word txt10
        .word 0
    TxtConcatCSES:                      ; 11
        .word txt11
        .word 0
    TermEscreverC:                      ; 12
        .word term1
        .word 0
    TermEscrever:                       ; 13
        .word term2
        .word 0
    TermEscreverEnter:                  ; 14
        .word term3
        .word 0
    TermEscreverL:                      ; 15
        .word term4
        .word 0
    MemLivre256:                        ; 16
        .word mem1
        .word 0
    MemAlocar256:                       ; 17
        .word mem2
        .word 0
    MemLiberar256:                      ; 18
        .word mem3
        .word 0
    TermEscreverNro:                    ; 19
        .word term5Inicio
        .word 0
    TermEscreverNroEspaco:              ; 20
        .word term5
        .word 0
    UnidadesPonteiro:                   ; 21
        .word unidades2
        .word 0
    UnidadesLerBloco512:                ; 22
        .word unidades3
        .word 0
    UnidadesGravarBloco512:             ; 23
        .word unidades4
        .word 0
    UnidadesChamar:                     ; 24
        .word unidades8
        .word 0
    UnidadesLerCampo8:                  ; 25
        .word unidades5
        .word 0
    UnidadesLerCampo16:                 ; 26
        .word unidades6
        .word 0
    UnidadesLerCampo32:                 ; 27
        .word unidades7
        .word 0
    HUSISLerVersao:                     ; 28
        .word prog2
        .word 0
    HUSISVerificarVersao:               ; 29
        .word prog3
        .word 0
    .word FimExportar
Importar:
    InicialEscreverNro:
        .word 0
        .word 0
    InicialEscreverDS:
        .word 0
        .word 0
    InicialEscreverC:
        .word 0
        .word 0
    InicialLerBloco:
        .word 0
        .word 0
    InicialCarregarArquivo:
        .word 0
        .word 0
    .word FimImportar

SistemaInicialSeg:
    .word 0
GlobalMemPos:
    .word 0
GlobalMemSeg:
    .word 0
GlobalMemTam:
    .word 0
GlobalMemBlocos:
    .word 0
GlobalProcessoAtual:
    .word 1
GlobalUnidadesPos:
    .word 0

Inicial:
    ; Armazenando os ponteiros recebidos
    mov InicialEscreverNro, dx
    mov InicialEscreverDS, cx
    mov InicialCarregarArquivo, bx
    mov InicialLerBloco, ax
    mov InicialEscreverC, di
    mov ax, es
    mov InicialEscreverNro + 2, ax
    mov InicialEscreverDS + 2, ax
    mov InicialCarregarArquivo + 2, ax
    mov InicialLerBloco + 2, ax
    mov InicialEscreverC + 2, ax
    mov SistemaInicialSeg, si
    push cs
    pop es

    mov ax, #0x6
    int #0x10

    push cs
    pop ArrumarExportar + 2
    callf ArrumarExportar

    callf TermEscrever
    .asciz "HUSIS v"
    
    mov ax, #Versao
    callf TermEscreverNro
    
    callf TermEscrever
    .asciz "."
    
    mov ax, #SubVersao
    callf TermEscreverNro
    
    callf TermEscrever
    .asciz " R"
    
    mov ax, #Revisao
    callf TermEscreverNro

    call memIniciar

    callf TermEscrever
    .asciz " [RAM: "

    callf MemLivre256
    shr ax
    shr ax
    callf TermEscreverNro
    callf TermEscrever
    .asciz "/"

    mov ax, GlobalMemBlocos
    shr ax
    shr ax
    callf TermEscreverNro
    callf TermEscreverL
    .asciz " KiB]"

    callf TermEscrever
    .ascii " Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)"
    .byte 13
    .byte 10
    .ascii " http://humbertocsjr.dev.br/"
    .byte 13
    .byte 10
    .byte 10
    .ascii " "
    .byte 0

    callf MemLivre256
    callf TermEscreverNroEspaco

    call unidadesIniciar
    
    callf MemLivre256
    callf TermEscreverNroEspaco

    retf

; ArrumarExportar
; Arruma a estrutura exportar de um binario
; es:0 = Binario
prog1:
    push si
    push ax
    push bx
    seg es
    mov si, 0x110
    push es
    pop bx
    prog1Arruma:
        seg es
        lodsw
        cmp ax, #FimExportar
        je prog1Fim
        seg es
        mov [si], bx
        inc si
        inc si
        jmp prog1Arruma
    prog1Fim:
    pop bx
    pop ax
    pop si
    retf

; HUSISLerVersao
; ret: ax: Versao
;      bx: SubVersao
;      cx: Revisao
prog2:
    mov ax, #Versao
    mov bx, #SubVersao
    mov cx, #Revisao
    retf

; HUSISVerificarVersao
; ax: Versao Minima
; bx: SubVersao Minima
; cx: Revisao Minima
; ret: cf: 1=Ok | 0=Incompativel
prog3:
    cmp ax, #Versao
    jb prog3Incompativel
    cmp bx, #SubVersao
    jb prog3Incompativel
    cmp bx, #Revisao
    jb prog3Incompativel
    stc
    retf
    prog3Incompativel:
        clc
        retf

.include /HUSIS/Nucleo.rag/txt.s
.include /HUSIS/Nucleo.rag/term.s
.include /HUSIS/Nucleo.rag/mem.s
.include /HUSIS/Nucleo.rag/unidades.s
.include /HUSIS/Nucleo.rag/saminix.s

Fim:
