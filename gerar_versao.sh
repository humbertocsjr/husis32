#!/bin/sh

echo '/* NAO ALTERAR GERADO AUTOMATICAMENTE */' > HUSIS/Nucleo.prg/versao.h
echo '#define COMPILACAO '$(cat compilacao) >> HUSIS/Nucleo.prg/versao.h
echo 'const uint16_t husis_versao = '$(date +'%y')';' >> HUSIS/Nucleo.prg/versao.h
echo 'const uint16_t husis_sub_versao = '$(date +'%m')';' >> HUSIS/Nucleo.prg/versao.h
echo 'const uint16_t husis_revisao = '$(date +'%d')' * 10 + COMPILACAO;' >> HUSIS/Nucleo.prg/versao.h
echo 'const txt_t husis_nome = "HUSIS v'$(date +'%y')'.'$(date +'%m')' R'$(date +'%d')$(cat compilacao)'";' >> HUSIS/Nucleo.prg/versao.h
