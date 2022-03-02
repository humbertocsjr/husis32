; Setor Inicial
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.27 - Versao inicial (Carrega a etapa 2)
; - 22.02.28 - Fornece API para a Etapa 2
;
; Recursos:
; - Carrega arquivos de ate 64KiB (Recomendavel)
; - Fornece uma API simplificada para proxima etapa
; - Realoca e calcula automaticamente a posicao na memoria
;
; Limitacoes:
; - Diretorios com ate 7 KiB de registros (224 sub-itens)
;

; Por padrão a BIOS define o CS(Ponteiro do Executavel) como 0
; Este codigo inicial foi feito para executar com CS 0x7c0

; EXECUTA COM CS=0x0000

; Define a pilha do processador para o endereço 0x600
cld
cli
mov ax, #0x600
mov ss, ax
mov sp, #4096
sti

; Armazena o ID do Disco inicial na pilha
xorb dh, dh
push dx

; Corrigindo os registradores de Segmento
mov ax, #0x7c0
mov es, ax
push es
pop ds

; Pulando para o inicio do código no segmento 0x7c0
push es
mov ax, #inicial2
push ax
retf

; EXECUTA COM CS=0x7c0

inicial2:
    ; Tira da pilha o ID do Disco armazenando numa constante
    pop GlobalDisco

    ; Escreve o texto inicial
    call TermEscreva
    .asciz "Setor Inicial"

    ; Chama a BIOS para pegar o tamanho da Memoria RAM em KiBs
    xor ax, ax
    int #0x12

    call TermEscrevaPonto

    ; Converte o tamanho de KiB para segmentos (16 bytes cada)
    mov bx, #1024 / 16
    mul bx
    dec ax

    ; Calcula a posicao final desse setor inicial para o topo da RAM
    sub ax, #1024 / 16                  ; O setor inicial completo tem 1 KiB

    ; Define o setor calculado como destino (es)
    mov es, ax

    ; Aponta para o inicio do destino em 0 (es:bx)
    xor bx, bx

    ; Informa que deseja ler o bloco 0
    xor ax, ax

    call TermEscrevaPonto

    ; Carrega o primeiro bloco de 1 KiB (sendo 512 desse + 512 da extensao)
    call DiscoLeia

    call TermEscrevaPonto

    ; Pula a execucao para o bloco carregado
    push es
    pop ds
    push es
    mov ax, #inicial3
    push ax
    retf

; EXECUTA COM CS=Topo da RAM - 1 KiB

inicial3:
    call TermEscrevaOk

    call TermEscreva
    .asciz "MFS"

    ; Calcula abaixo desse segmento atual - 1 KiB como memoria temporaria
    ;  para armazenar blocos do disco
    mov ax, es
    sub ax, #1024 / 16
    mov GlobalDiscoTemp, ax

    ; Calcula 7 KiB abaixo do anterior para armazenar um diretorio
    sub ax, #7 * 1024 / 16
    mov GlobalDirTemp, ax

    ; Calcula 1 KiB abaixo do anterior para armazenar dados do arquivo
    sub ax, #1024 / 16
    mov GlobalArqTemp, ax

    ; Define destino (es) como segmento temporario do disco
    push GlobalDiscoTemp
    pop es

    ; Define ponteiro destino para o inicio (es:bx)
    xor bx, bx

    ; Le o bloco 1 (Bloco Descritivo do MinixFS)
    mov ax, #1

    call TermEscrevaPonto

    ; Carrega o bloco 1
    call DiscoLeia

    call TermEscrevaPonto

    ; Copia a Assinatura de 16bits que esta na posicao 16 (es:16)
    seg es
    mov ax, 16

    ; Verifica a versao do MinixFS
    cmp ax, #4991                       ; MINIXFS 1 14 Caracteres
    jne inicial3Nao14
        ; Define o tamanho de um registro de diretorio
        mov GlobalTamRegistro, #16
        jmps inicial3Continua
    inicial3Nao14:

    cmp ax, #5007                       ; MINIXFS 1 30 Caracteres
    jne inicial3Nao30
        ; Define o tamanho de um registro de diretorio
        mov GlobalTamRegistro, #32
        jmps inicial3Continua
    inicial3Nao30:

    ; Caso nao seja compativel com essa implementacao do MinixFS da erro
    call TermEscreva
    .asciz "[ Erro ]"
    jmp inicial3Infinito

    inicial3Continua:

        call TermEscrevaPonto

        ; Le a quantidade de itens (es:0)
        seg es
        mov ax, 0                           ; Qtd de Itens
        mov GlobalItensQtd, ax

        ; Calcula a quantidade * tamanho de um item / tamanho de um bloco
        mov bx, #32
        mul bx
        mov bx, #1024
        div bx

        ; Armazena o total de blocos com itens
        mov GlobalItensBlocos, ax

        ; Le a posicao do final do cabecalho do MinixFS/Inicio dos dados comuns
        seg es
        mov ax, 8                           ; Inicio dos dados
        mov GlobalInicioDados, ax

        ; Calcula o inicio da tabela de itens
        ; = Blocos do Mapa de Itens + Blocos do Mapa de Zonas Ocupadas
        ;   + 1 Bloco do Setor Inicial
        ;   + 1 Bloco do Descritivo do Sistema de Arquivos
        seg es
        mov ax, 4                           ; Qtd de Blocos dos Itens
        seg es
        add ax, 6                           ; Qtd de Blocos das Zonas
        inc ax
        inc ax

        ; Armazena  a posicao da tabela de itens
        mov GlobalItensPos, ax

        call TermEscrevaPonto

        ; Le a quantidade de blocos da tabela de itens
        mov cx, GlobalItensBlocos       ; Usado abaixo para ler os blocos

        ; Calcula o tamanho da tabela de itens para reservar memoria ram
        mov ax, #1024 / 16
        mul cx
        push ax
        pop bx

        ; Calcula o segmento da memoria ram que vai ser utilizado
        ; = Posicao de Armazenamento de arquivos - Tamanho da Tabela
        mov ax, GlobalArqTemp
        sub ax, bx
        mov GlobalItensTemp, ax

        ; Define o destino como a Tabela de Itens
        mov es, ax
        mov ax, GlobalItensPos

        ; Define o ponteiro para o inicio do segmento (es:bx)
        xor bx, bx

        ; Carrega sequencialmente todos os blocos
        inicial3Carregar:
            call TermEscrevaPonto

            ; Carrega um bloco
            call DiscoLeia

            ; Adiciona no ponteiro 1 KiB (es:bx)
            add bx, #1024

            ; Vai para a proxima posicao
            inc ax

            ; Repete decrementando quantidade de blocos (cx) ate zerar
            loop inicial3Carregar

        ; Continua na etapa 4, na Extensao de 512 bytes
        jmp inicial4

; Loop infinito que desliga o processador
inicial3Infinito:
    hlt                                 ; Suspende o processador
    jmp inicial3Infinito

; Rotinas extras de manipulacao do disco e do terminal (tela)

.include disco.s
.include term.s

; Exatamente na posicao 490 do armazena os parametros alteraveis pelo software
;  que grava esse setor no inicio do disco
.align 490
.ascii "HC"
ParamSetores:
    .word 9                             ; Qtd. de setores em cada lado do disco
ParamCabecas:
    .word 2                             ; Qtd. de faces de discos no HD
ParamTrilhas:
    .word 40                            ; Qtd. de trilhas em um disco
ParamNome:
    .asciz "/Etapa2.bin"                ; Nome do arquivo da Etapa 2

; Assinatura de disco inicializavel
.align 510
.word 0xaa55

; Constantes Globais
GlobalDisco:
    .word 0
GlobalTamRegistro:
    .word 0
GlobalItensTemp:
    .word 0
GlobalItensPos:
    .word 0
GlobalItensQtd:
    .word 0
GlobalItensBlocos:
    .word 0
GlobalDiscoTemp:
    .word 0
GlobalDiscoPos:
    .word 0
GlobalDirTemp:
    .word 0
GlobalDirPos:
    .word 0
GlobalDirQtd:
    .word 0
GlobalArqTemp:
    .word 0
GlobalArqPos:
    .word 0
GlobalDestino:
    .word 0
GlobalDestinoSeg:
    .word 0
GlobalInicioDados:
    .word 0
GlobalTamanho:
    .word 0

; Extensao do codigo de terminal usado na extensao e na Etapa 2
.include termext.s

; EXECUTA COM CS=Topo da RAM - 1 KiB

inicial4:

    call TermEscrevaOk

    ; Calcula a posicao que ficara a Etapa 2 e sua conf. (Tamanho max. 16 KiB)
    seg cs
    mov ax, GlobalItensTemp
    sub ax, #16 * 1024 / 16             ; Reserva 16 KiB para Etapa 2

    ; Grava na variavel global (a partir de agora o DS pode nao apontar para o CS)
    seg cs                              ; Forcando que o proximo comando use o CS
    mov GlobalDestinoSeg, ax

    ; Definindo o destino como posicao onde ficara a Etapa 2
    mov es, ax
    mov si, #ParamNome

    ; Carrega o arquivo que esta na constante ParamNome
    call inicial4CarregaArq

    ; Apos carregado define os parametros entregues para a Etapa 2
    seg cs
    mov ax, GlobalDestinoSeg
    mov ds, ax                          ; ds = cs da Etapa 2
    mov ax, cs
    mov es, ax                          ; es = cs desse setor inicial
    mov ax, #ExportarLeia               ; es:ax = Rotina Ler Disco
    mov bx, #ExportarCarregaArq         ; es:bx = Rotina Carregar Arq.
    mov cx, #ExportarEscrevaDS          ; es:cx = Rotina Escrever na Tela
    mov dx, #ExportarEscrevaNro         ; es:dx = Rotina Escrever Numero
    mov di, #ExportarEscrevaC           ; es:di = Rotina Escrever Caractere
    xor si, si

    ; Executa a Etapa 2
    push ds
    push si
    retf

    ; Sub rotina que carrega um arquivo
    inicial4CarregaArq:
        ; Guarda na pilha os registradores para preserva-los
        push ds
        push si
        push di
        push cx
        push bx
        push dx

        ; Define o destino como es
        mov ax, es
        seg cs
        mov GlobalDestino, ax

        ; Define o tamanho inicial como 0
        xor ax, ax
        seg cs
        mov GlobalTamanho, ax

        ; Busca o arquivo usando o endereco em ds:si (ParamNome)
        call MinixFSBuscarItem

        ; OPCIONAL: Imprime o ID do item
        ;call TermEscreva
        ;.asciz "ID:"
        ;call TermEscrevaNro

        ; Gera um ponteiro em es:di para o item na tabela de itens
        call MinixFSPonteiroItem

        ; Iguala o ds:si com o es:di, para que o es:di seja usado para Etapa 2
        mov ax, es
        mov ds, ax
        mov si, di

        ; Pula para a parte do item que tem os ponteiro de onde estao os dados
        add si, #0xe

        ; Carrega diretamente os primeiros 7 blocos
        mov cx, #7
        inicial4Carrega:
            ; Le um ponteiro para um bloco no disco
            lodsw

            ; Se for 0 chegou ao fim
            cmp ax, #0
            je inicial4Fim:

            ; Le o Destino em es
            seg cs
            push GlobalDestino
            pop es

            ; Define o ponteiro para 0 (es:bx)
            xor bx, bx

            ; Le um bloco
            call DiscoLeia

            ; Incrementa o tamanho (Guardara o tamanho em blocos)
            seg cs
            inc GlobalTamanho

            ; Calcula o novo destino = destino + 1 KiB
            seg cs
            add GlobalDestino, #1024 / 16

            ; Repete ate CX = 0
            loop inicial4Carrega
        ; Le o proximo ponteiro (Indireto)
        lodsw

        ; Na segunda vez que passar o proximo valor sera 0, portanto ignorando
        ;  para funcionar o arquivo DEVE ter menos que 512+7 blocos de 1 KiB

        ; Caso seja 0 terminou de carregar
        cmp ax, #0
        je inicial4Fim

        ; Caso exista um ponteiro indireto, vai carregar o bloco onde ficam
        ;  os ponteiros diretos e reinicia a rotina de carregamento

        ; Devolve o segmento da tabela de itens para o es
        push ds
        pop es

        ; Define o ponteiro para 0 (es:bx)
        xor bx, bx

        ; Carrega o bloco de ponteiros
        call DiscoLeia

        ; Define o inicio da lista como 0
        xor si, si

        ; Define como 512 ponteiros possiveis no bloco
        mov cx, #512

        ; Retorna a rotina que carrega os blocos
        jmp inicial4Carrega

        ; Termina o carregamento
        inicial4Fim:

        ; Escreve na tela o tamanho lido e encerra
        seg cs
        mov ax, GlobalTamanho
        call TermEscrevaNro
        call TermEscreva
        .asciz " KiB "
        call TermEscrevaOk

        ; Devolve os registradores originais
        pop dx
        pop bx
        pop cx
        pop di
        pop si
        pop ds

        ; Retorna a quem chamou a rotina
        ret

; Adaptacoes das rotinas locais para rotinas exportaveis a diferenca eh o
;  comando de retorno e a forma de chamar.
; Uma rotina local se chama com call e retorna com ret enquanto uma rotina
;  exportavel se chama com callf e retorna com retf.
; Alem dessa diferenca pratica de codigo, estes comandos locais usam um
;  ponteiro para armazenar o destino e retorno, sendo que a rotina exportavel
;  armazena o ponteiro e o segmento do destino e retorno, podendo dar saltos
;  longos na memoria ram.
ExportarLeia:
    call DiscoLeia
    retf
ExportarEscrevaDS:
    call TermEscrevaDS
    retf
ExportarEscrevaNro:
    call TermEscrevaNroEspaco
    retf
ExportarCarregaArq:
    call inicial4CarregaArq
    retf
ExportarEscrevaC:
    call TermEscrevaC
    retf

; Busca um item pelo sistema de arquivos usando o enddereco completo dele
; ds:si = Nome do item
; ret: es:di = Item
MinixFSBuscarItem:
minix1:
    push si
    push bp
    mov bp, sp
    call TermEscrevaDS
    call TermEscrevaPonto
    lodsb
    cmpb al, #'/'
    je minix1EnderecoOk
        jmp minix1EnderecoErrado
    minix1EnderecoOk:
    mov ax, #1
    minix1Busca:
        ;call TermEscreva
        ;.asciz "ID:"
        ;call TermEscrevaNro
        push si
        seg cs
        mov GlobalDirPos, ax
        call MinixFSPonteiroItem
        add di, #0xe                    ; Zona 0
        xor bx, bx
        mov cx, #7
        minix1CarregaDir:
            seg es
            mov ax, [di]
            cmp ax, #0
            je minix1FimDir
            push es
            push di
            seg cs
            push GlobalDirTemp
            pop es
            call DiscoLeia
            add bx, #1024
            pop di
            pop es
            add di, #2
            loop minix1CarregaDir
        minix1FimDir:
        mov ax, bx
        seg cs
        mov bx, GlobalTamRegistro
        xor dx, dx
        div bx
        seg cs
        mov GlobalDirQtd, ax
        mov cx, ax
        seg cs
        push GlobalDirTemp
        pop es
        xor di, di
        xor dx, dx
        minix1SubItens:
            pop si
            push si
            push di
            push cx
            add di, #2
            seg cs
            mov cx, GlobalTamRegistro
            dec cx
            dec cx
            minix1Compara:
                lodsb
                cmpb al, #'|'
                jne minix1SemBarra
                    xorb al, al
                minix1SemBarra:
                cmpb al, #'/'
                jne minix1ContCompara
                    mov dx, #1
                    seg es
                    cmpb [di], #0
                    jne minix1ContCompara
                    jmp minix1Encontrado
                minix1ContCompara:
                seg es
                cmpb al, [di]
                jne minix1Prox
                xor dx, dx
                cmpb al, #0
                je minix1Encontrado
                inc di
                dec cx
                cmp cx, #0
                je minix1Encontrado
                jmp minix1Compara
            minix1Prox:
            pop cx
            pop di
            seg cs
            add di, GlobalTamRegistro
            dec cx
            cmp cx, #0
            je minix1EnderecoErrado
            jmp minix1SubItens
        minix1Encontrado:
        pop cx
        pop di
        cmp dx, #0
        je minix1Fim
        seg es
        mov ax, [di]
        jmp minix1Busca
    minix1Fim:
    seg es
    mov ax, [di]
    mov sp, bp
    pop bp
    pop si
    ret
    minix1EnderecoErrado:
        call TermEscreva
        .asciz " [ Caminho Invalido ]"
        jmp inicial3Infinito

MinixFSPonteiroItem:
minix2:
    push ax
    push bx
    dec ax
    seg cs
    push GlobalItensTemp
    pop es
    mov bx, #32
    mul bx
    mov di, ax
    seg cs
    add ax, GlobalInicioDados
    pop bx
    pop ax
    ret

; Forca que o arquivo tenha exatos 1 KiB, acredite nao existe muito espaco
;  para modificacao desse codigo, esse marcador so existe para praticidade.
.align 1024
