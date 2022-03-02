; Traducao da Etapa 2 para PTBR
; Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
;
; Historia:
; - 22.02.28 - Versao inicial

.ascii "TRAD"
.ascii "PTBR"
.word Fim
; Inicio dos itens
.word TradEtapa2
.word TradOk
.word TradArqConfig
.word TradOpcoes
.word TradEscolha
.word TradAuto
.word TradManual
.word TradIniciando
.word TradEncerrado

TradEtapa2:
    .asciz "Etapa 2"
TradOk:
    .asciz " [ OK ]"
TradArqConfig:
    .asciz "/HUSIS/SistemaInicial.rag/etapa2.cfg"
TradOpcoes:
    .asciz "Sistemas Operacionais"
TradEscolha:
    .asciz "Escolha uma opcao acima ou aguarde: "
TradAuto:
    .asciz "Modo Auto"
TradManual:
    .asciz "Modo Manual: "
TradIniciando:
    .asciz "Iniciando "
TradEncerrado:
    .asciz "[ ROTINA INICIAL ENCERRADA. DESLIGUE OU REINICIE O COMPUTADOR ]"

Fim:
