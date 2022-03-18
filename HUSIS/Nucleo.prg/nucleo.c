/*
 * HUSIS - Humberto Sistema Operacional
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.16 - Rotina que inicia a memoria RAM
 */

#include "husis.h"
#include "versao.h"
#include "es.h"
#include "iut.h"
#include "processo.h"
#include "mem.h"
#include "dispositivo.h"

iut_controle_t _obj_janela;
iut_controle_t _obj_conteudo1;
iut_controle_t _obj_conteudo2;
iut_controle_t _obj_progresso;

void husis_progresso(txt_t texto1, txt_t texto2)
{
    iut_altera_texto(&_obj_conteudo1, texto1);
    iut_altera_texto(&_obj_conteudo2, texto2);
}

void husis_progresso_altera(int8_t percentual)
{
    iut_altera_valor(&_obj_progresso, percentual);
}

void husis(txt_t args)
{
    tam_t tam = 0;
    es_video_inicia(COR_CIANO, COR_PRETO);
    iut_inicia();
    
    iut_nova_janela(&_obj_janela, 5, 5, 70, 15, husis_nome, IUT_BOTAO_NENHUM);
    iut_novo_rotulo(&_obj_conteudo1, 0, 0, 68, "");
    iut_adiciona(&_obj_janela, &_obj_conteudo1);
    iut_novo_rotulo(&_obj_conteudo2, 0, 1, 68, "");
    iut_adiciona(&_obj_janela, &_obj_conteudo2);
    iut_novo_progresso(&_obj_progresso, 0, 9, 68, 0);
    iut_adiciona(&_obj_janela, &_obj_progresso);
    iut_redesenha(&_obj_janela);
    
    husis_progresso_altera(10);
    husis_progresso("Iniciando . . .", "");
    mem_inicia();
    
    tam = iut_desenha_rotulo(0,0, 13, "RAM Inicial: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,0, 10, mem_tamanho() / 1024, COR_CIANO_CLARO, COR_PRETO);
    tam += iut_desenha_rotulo(tam, 0, 14, " KiB / Livre: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,0, 10, mem_livre() / 1024, COR_CIANO_CLARO, COR_PRETO);
    tam += iut_desenha_rotulo(tam, 0, 11, " KiB (Max: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,0, 10, MEM_TOTAL_PAGINAS / 256, COR_CIANO_CLARO, COR_PRETO);
    tam += iut_desenha_rotulo(tam, 0, 5, " MiB)", COR_CIANO, COR_PRETO);
    
    tam = iut_desenha_rotulo(0,1, 8, "Nucleo: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,1,10, mem_tamanho_processo(PROCESSO_NUCLEO) / 1024, COR_CIANO_CLARO, COR_PRETO);
    tam += iut_desenha_rotulo(tam,1, 15, " KiB | Inicio: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,1,10, es_nucleo_primeira_pagina() * 4, COR_CIANO_CLARO, COR_PRETO);
    tam += iut_desenha_rotulo(tam,1, 15, " KiB | Fim: ", COR_CIANO, COR_PRETO);
    tam += iut_desenha_rotulo_nro(tam,1,10, es_nucleo_ultima_pagina() * 4, COR_CIANO_CLARO, COR_PRETO);
    iut_desenha_rotulo(tam, 1, 4, " KiB", COR_CIANO, COR_PRETO);
    
    tam = iut_desenha_rotulo(0,2,9,"Comando: ", COR_CIANO, COR_PRETO);
    iut_desenha_rotulo(tam, 2, 70, args, COR_CIANO_CLARO, COR_PRETO);
    
    husis_progresso_altera(20);
    husis_progresso("Iniciando Gerenciador de Dispositivos . . .", "");
    dispositivo_inicia();
    husis_progresso_altera(25);
    husis_progresso("Iniciando Controladores de Disco . . .", "");
    dispositivo_disco_inicia();
    
    tam = iut_desenha_rotulo(0,2,39,"Quantidade de Dispositivos Carregados: ", COR_CIANO, COR_PRETO);
    iut_desenha_rotulo_nro(tam, 2, 70, dispositivo_quantidade(), COR_CIANO_CLARO, COR_PRETO);
    
}
