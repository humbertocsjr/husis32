#ifndef MEM_H
#define MEM_H
#include "husis.h"

void mem_inicia();
posicao_t mem_aloca(processo_t processo, uint8_t qtd_paginas);
void mem_libera(posicao_t posicao);
void mem_libera_processo(processo_t processo);
status_t mem_grava(posicao_t pagina, processo_t processo, uint8_t posicao);
tam_t mem_livre();
tam_t mem_tamanho();
#endif
