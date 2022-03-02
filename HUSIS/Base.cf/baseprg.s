.ascii "HC"                             ; 00 Assinatura
.word 1                                 ; 02 Versao da API
.word Inicial                           ; 04 Rotina inicial
.word Fim                               ; 06 Ponteiro para o fim do arquivo
.word Versao                            ; 08 Versao do Programa
.word SubVersao                         ; 10 SubVersao do Programa
.word Revisao                           ; 12 Revisao do Programa
.word Pilha                             ; 14 Tamanho da Pilha
.word Exportar                          ; 16 Lista de rotinas exportadas
Argumentos:
.word 0                                 ; 18 Segmento onde esta a lista de
                                        ;    argumentos
.word 0                                 ; 20 Reservado para o HUSIS
.word 0                                 ; 22 Reservado para o HUSIS
.word 0                                 ; 24 Reservado para o HUSIS
.word 0                                 ; 26 Reservado para o HUSIS
.word 0                                 ; 28 Reservado para o HUSIS
.word 0                                 ; 30 Reservado para o HUSIS
; 32 Inicio do executável

FimSeg = Fim / 16 + 1
FimExportar = 0
FimImportar = 0
