; Unidades de Disco
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.03.07 - Versao inicial
;
; As unidades sao definidas pelos controladores de disco, que as geram apenas
; preenchidas os campos LerBloco512, GravarBloco512 e TotalBlocos512, tambem 
; define o Status como StatusUnidadeExiste e o Tipo como o correspondente.
;
; Caso o TotalBlocos512 seja 0, o disco tem seu tamanho dinamico.
;
; 

ObjUnidadeStatus = 0
    ; Numero inteiro de 8 bit
ObjUnidadeTipo = 1
    ; Numero inteiro de 8 bits
ObjUnidadeLerBloco512 = 2
    ; Le um bloco
    ; dx:ax: Endereco
    ; es:di: Destino
    ; ret: cf: 1=Ok | 0=Ocorreu um erro ao tentar ler
ObjUnidadeGravarBloco512 = 6
    ; Grava um bloco
    ; dx:ax: Endereco
    ; es:di: Origem
    ; ret: cf: 1=Ok | 0=Ocorreu um erro ao tentar gravar
ObjUnidadeTotalBlocos512 = 10
    ; Numero inteiro de 32 bits com o total de blocos
ObjUnidadeIdRaiz = 14
    ; Numero inteiro de 16 bits com o Id da Raiz
ObjUnidadeListarItem = 16
    ; Lista um item do diretorio
    ; ax: Id do Diretorio
    ; es:di: Destino 64 bytes ObjItem (Gerenciado pelo chamador)
    ; cx: Posicao
ObjUnidadeAbrirArq = 20
    ; Abre um arquivo
    ; ax: Id do Arquivo
ObjUnidadeFecharArq = 24
    ; Fecha um arquivo aberto
    ; ax: Id do Arquivo
ObjUnidadeLer = 28
    ; Le um trecho do arquivo
    ; ax: Id do Arquivo
    ; cx: Quantidade de bytes
    ; es:di: Destino
ObjUnidadeGravar = 32
    ; Grava um trecho no arquivo
    ; ax: Id do Arquivo
    ; cx: Quantidade de bytes
    ; es:di: Origem
ObjUnidadeCalcTamArq = 36
    ; Calcula o tamanho de um arquivo
    ; ax: Id do Arquivo
    ; ret: cf: 1=Ok | 0=Arquivo nao existe
    ;      ds:ax: Tamanho
ObjUnidadeDesmontar = 40
    ; Desmonta uma unidade
    ; es:di: Unidade
ObjUnidadeReservadoDisp = 44
    ; Uso do controlador do dispositivo (10 bytes)
ObjUnidadeReservadoSisArq = 54
    ; Uso do sistema de arquivos (10 bytes)
ObjUnidadeNome = 64
    ; Nome da Unidade
ObjUnidadeNomeDispositivo = 96
    ; Nome do Dispositivo dessa Unidade
TamObjUnidade = 128
QtdPorBlocoObjUnidade = 256 / TamObjUnidade
BlocosListaObjUnidade = 8
QtdListaObjUnidade = BlocosListaObjUnidade * QtdPorBlocoObjUnidade

StatusUnidadeNaoExiste = 0
StatusUnidadeExiste = 1
StatusUnidadeMontado = 2

TipoUnidadeDesconhecido = 0
TipoUnidadeRemovivel = 1
TipoUnidadeFixa = 2
TipoUnidadeRemota = 3

unidadesIniciar:
;unidades1
    push ax
    push cx
    push di
    push es
    mov cx, #BlocosListaObjUnidade
    callf MemAlocar256
    jnc unidades1Fim
        mov ax, es
        mov GlobalUnidadesPos, ax
        mov cx, #TamObjUnidade * QtdListaObjUnidade
        xor ax, ax
        xor di, di
        rep
        stosb
        stc
    unidades1Fim:
    pop es
    pop di
    pop cx
    pop ax
    ret

; UnidadesPonteiro
; bx: Id da Unidade
; ret: es:di: Unidade
;      cf: 1=Ok | 0=Nao existe
unidades2:
    push ax
    push bx
    cmp bx, #QtdListaObjUnidade
    jb unidades2Ok
        clc
        jmps unidades2Fim
    unidades2Ok:
    seg cs
    mov ax, GlobalUnidadesPos
    mov es, ax
    mov ax, #TamObjUnidade
    mul bx
    mov di, ax
    stc
    unidades2Fim:
    pop bx
    pop ax
    retf

; UnidadesLerBloco512
; dx:ax: Endereco
; es:di: Destino
; bx: Unidade
; ret: cf: 1=Ok | 0=Ocorreu um erro ao tentar ler
unidades3:
    push si
    mov si, #ObjUnidadeLerBloco512
    callf UnidadesChamar
    pop si
    retf

; UnidadesGravarBloco512
; dx:ax: Endereco
; es:di: Origem
; bx: Unidade
; ret: cf: 1=Ok | 0=Ocorreu um erro ao tentar gravar
unidades4:
    push si
    mov si, #ObjUnidadeGravarBloco512
    callf UnidadesChamar
    pop si
    retf
        
; UnidadesLerCampo8
; Le um campo de 8bits
; bx: Unidade
; cx: Campo
; ret: cf: 1=Ok | 0=Unidade nao existe
;      al: Valor
;      ah: 0
unidades5:
    push es
    push di
    push bx
    xor ax, ax
    callf UnidadesPonteiro
    jnc unidades5Fim
        cmp cx, #TamObjUnidade
        jb unidades5Ok
            clc
            jmp unidades5Fim
        unidades5Ok:
        mov bx, cx
        seg es
        movb al, [bx_di]
        stc
    unidades5Fim:
    pop bx
    pop di
    pop es
    retf

; UnidadesLerCampo16
; Le um campo de 16bits
; bx: Unidade
; cx: Campo
; ret: cf: 1=Ok | 0=Unidade nao existe
;      ax: Valor
unidades6:
    push es
    push di
    push bx
    xor ax, ax
    callf UnidadesPonteiro
    jnc unidades6Fim
        cmp cx, #TamObjUnidade - 1
        jb unidades6Ok
            clc
            jmp unidades6Fim
        unidades6Ok:
        mov bx, cx
        seg es
        mov ax, [bx_di]
        stc
    unidades6Fim:
    pop bx
    pop di
    pop es
    retf

; UnidadesLerCampo32
; Le um campo de 32bits
; bx: Unidade
; cx: Campo
; ret: cf: 1=Ok | 0=Unidade nao existe
;      dx:ax: Valor
unidades7:
    push es
    push di
    push bx
    xor ax, ax
    xor dx, dx
    callf UnidadesPonteiro
    jnc unidades7Fim
        cmp cx, #TamObjUnidade - 3
        jb unidades7Ok
            clc
            jmp unidades7Fim
        unidades7Ok:
        mov bx, cx
        seg es
        mov ax, [bx_di]
        seg es
        mov dx, 2[bx_di]
        stc
    unidades7Fim:
    pop bx
    pop di
    pop es
    retf

; UnidadesChamar
; bx: Unidade
; si: Campo
; ax:cx:dx:di:es: Valores passados
; ret: Depende do item chamado
unidades8:
    push bp
    mov bp, sp
    sub sp, #20
    mov -2[bp], ax
    mov -4[bp], es
    mov -6[bp], di
    mov -8[bp], cx
    mov -10[bp], bx
    mov -12[bp], dx
    mov cx, si
    callf UnidadesLerCampo32
    jc unidades8Ok
        mov cx, -8[bp]
        clc
        jmp unidades8Fim
    unidades8Ok:
    cmp ax, #0
    jne unidades8Existe:
        mov cx, -8[bp]
        mov ax, -2[bp]
        mov dx, -12[bp]
        clc
        jmp unidades8Fim
    unidades8Existe:
    push cs
    seg cs
    push unidades8Ponteiro
    push dx
    push ax
    mov ax,-4[bp]
    mov es, ax
    mov ax, -2[bp]
    mov di, -6[bp]
    mov cx, -8[bp]
    mov bx, -10[bp]
    mov dx, -12[bp]
    retf
    unidades8Fim:
    mov sp, bp
    pop bp
    retf
    unidades8Ponteiro:
        .word unidades8Fim
