; Sistema de Arquivos Minix
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.03.07 - Versao Prototipo inicial


; SAMinixMontar
; bx: Unidade
; ret: cf: 1=OK | 0=Ilegivel
saminix1:
    mov cx, #4                          ; 1 KiB = 4 Blocos
    callf MemAlocar256
    jnc saminix1Fim
    xor dx, dx
    mov ax, #1
    seg cs
    callf UnidadesLerBloco512
    jnc saminix1Fim:
        seg es
        mov ax, 16                      ; Assinatura do SAMinix
        cmp ax, #4991                   ; V1 14
        jne saminix1Nao14
        ; TODO: Falta criar o UnidadesGravar16 para gravar esse valor
        saminix1Nao14:
        stc
    saminix1Fim:
    retf

; SAMinixListarItem
; Lista um item do diretorio
; ax: Id do Diretorio
; es:di: Destino 64 bytes ObjItem (Gerenciado pelo chamador)
; cx: Posicao
saminix2:
    
    retf
