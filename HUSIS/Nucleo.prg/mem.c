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

status_t mem_aloca_pag_baixa(processo_t processo, uint8_t qtd_paginas, posicao_t * posicao)
{
    posicao_t pos = 0;
    posicao_t temp = 0;
    posicao_t qtd = 0;
    if(qtd_paginas == 0 | qtd_paginas > 255) return ERRO_ARGUMENTO_INVALIDO;
    for(posicao_t i = 1; i < 256; i++)
    {
        if(temp == 0)
        {
            if((_mem_mapa[i] & 0xff) == PROCESSO_VAZIO)
            {
                temp = i;
                qtd = 1;
            }
        }
        else
        {
            if((_mem_mapa[i] & 0xff) != PROCESSO_VAZIO)
            {
                temp = 0;
                qtd = 0;
            }
            else
            {
                qtd++;
            }
        }
        if(qtd == qtd_paginas)
        {
            pos = temp;
        }
    }
    for(posicao_t i = 0; i < qtd; i++)
    {
        if((_mem_mapa[i] & 0xff) != PROCESSO_VAZIO) return ERRO_DADOS_CORROMPIDOS;
        _mem_mapa[pos + i] = (processo & 0xff) | (i << 8);
    }
    posicao = pos;
    return OK;
}

status_t mem_aloca_pag(processo_t processo, uint8_t qtd_paginas, posicao_t * posicao)
{
    posicao_t pos = 0;
    posicao_t temp = 0;
    posicao_t qtd = 0;
    if(qtd_paginas == 0 | qtd_paginas > 255) return ERRO_ARGUMENTO_INVALIDO;
    for(posicao_t i = 256; i < MEM_TOTAL_PAGINAS; i++)
    {
        if(temp == 0)
        {
            if((_mem_mapa[i] & 0xff) == PROCESSO_VAZIO)
            {
                temp = i;
                qtd = 1;
            }
        }
        else
        {
            if((_mem_mapa[i] & 0xff) != PROCESSO_VAZIO)
            {
                temp = 0;
                qtd = 0;
            }
            else
            {
                qtd++;
            }
        }
        if(qtd == qtd_paginas)
        {
            pos = temp;
        }
    }
    for(posicao_t i = 0; i < qtd; i++)
    {
        if((_mem_mapa[i] & 0xff) != PROCESSO_VAZIO) return ERRO_DADOS_CORROMPIDOS;
        _mem_mapa[pos + i] = (processo & 0xff) | (i << 8);
    }
    posicao = pos;
    return OK;
}

status_t mem_libera_pag(posicao_t posicao)
{
    // Deve ser ponteiro para a primeira pagina alocada
    if((_mem_mapa[posicao] & 0xff00) != 0) return ERRO_ARGUMENTO_INVALIDO;
    // Deve ser o processo que alocou ou o nucleo
    if((_mem_mapa[posicao] & 0xff) != PROCESSO_NUCLEO & (_mem_mapa[posicao] & 0xff) != processo_atual()) return ERRO_ACESSO_NEGADO;
    // Libera as paginas
    posicao_t atual = 0;
    posicao_t proc = _mem_mapa[posicao] & 0xff;
    for(posicao_t i = posicao; i < MEM_TOTAL_PAGINAS; i++)
    {
        if(_mem_mapa[posicao] == (proc | (atual << 8)))
        {
            _mem_mapa[posicao] = 0 | (PROCESSO_VAZIO & 0xff);
            atual++;
        }
        else
        {
            break;
        }
    }
    return OK;
}

status_t mem_libera_processo(processo_t processo)
{
    // Deve ser um processo valido
    if(processo == PROCESSO_NUCLEO | processo == PROCESSO_VAZIO | processo == PROCESSO_RESERVADO) return ERRO_ARGUMENTO_INVALIDO;
    // Apenas o nucleo pode efetuar essa acao
    if(processo_atual() != PROCESSO_NUCLEO) return ERRO_ACESSO_NEGADO;
    // Libera as paginas
    for(posicao_t i = 1; i < MEM_TOTAL_PAGINAS; i++)
    {
        if((_mem_mapa[i] & 0xff) == processo)
        {
            _mem_mapa[i] = 0 | (PROCESSO_VAZIO & 0xff);
        }
    }
    return OK;
}
