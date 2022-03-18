/*
 * Gerenciamento de Memoria
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.15 - Versão inicial
 * - 22.03.17 - Leitura do tamanho da RAM pelo Multiboot
 */
#include "husis.h"
#include "mem.h"
#include "es.h"
#include "processo.h"
#include "iut.h"


uint16_t _mem_mapa[MEM_TOTAL_PAGINAS];

void mem_inicia()
{
    uint8_t tam = 0;
    husis_progresso("Gerenciador de Memoria", "Iniciando mapa. . .");
    for(posicao_t i = 0; i < MEM_TOTAL_PAGINAS; i++)
    {
        mem_grava(i, PROCESSO_RESERVADO, 0);
    }
    husis_progresso("Gerenciador de Memoria", "Calculando areas reservadas. . .");
    es_aplicar_areas_restritas(MEM_TOTAL_PAGINAS);
    husis_progresso("Gerenciador de Memoria", "Concluindo . . .");
    for(posicao_t i = 0; i < MEM_TOTAL_PAGINAS; i++)
    {
        if(i >= es_nucleo_primeira_pagina() & i <= es_nucleo_ultima_pagina())
        {
            mem_grava(i, PROCESSO_NUCLEO, tam);
            tam++;
        }
    }
}

status_t mem_grava(posicao_t pagina, processo_t processo, uint8_t posicao)
{
    if(processo_atual() != PROCESSO_NUCLEO) return ERRO_ACESSO_NEGADO;
    if(pagina >= MEM_TOTAL_PAGINAS) return ERRO_ESTOURO_DE_CAPACIDADE;
    _mem_mapa[pagina] = (processo & 0xff) | (posicao << 8 & 0xff);
}

status_t mem_le(posicao_t pagina, processo_t * processo, uint8_t * posicao)
{
    if(processo_atual() != PROCESSO_NUCLEO) return ERRO_ACESSO_NEGADO;
    if(pagina >= MEM_TOTAL_PAGINAS) return ERRO_ESTOURO_DE_CAPACIDADE;
    *processo = _mem_mapa[pagina] & 0xff;
    *posicao = _mem_mapa[pagina] >> 8 & 0xff;
}

tam_t mem_livre()
{
    return mem_tamanho_processo(PROCESSO_VAZIO);
}

tam_t mem_tamanho()
{
    return es_mem_tamanho();
}

tam_t mem_tamanho_processo(processo_t processo)
{
    tam_t tam = 0;
    for(posicao_t i = 0; i < MEM_TOTAL_PAGINAS; i++)
    {
        if((_mem_mapa[i] & 0xff) == processo)
        {
            tam += 4096;
        }
    }
    return tam;
}

posicao_t mem_aloca(processo_t processo, uint8_t qtd_paginas)
{
    
}

void mem_libera(posicao_t posicao)
{
}

void mem_libera_processo(processo_t processo)
{
}
