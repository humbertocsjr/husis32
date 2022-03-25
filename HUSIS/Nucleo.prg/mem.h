#ifndef MEM_H
#define MEM_H
#include "husis.h"

void mem_inicia();
status_t mem_aloca_pag_baixa(processo_t processo, uint8_t qtd_paginas, posicao_t * posicao);
status_t mem_aloca_pag(processo_t processo, uint8_t qtd_paginas, posicao_t * posicao);
status_t mem_libera_pag(posicao_t posicao);
status_t mem_libera_processo(processo_t processo);
status_t mem_grava(posicao_t pagina, processo_t processo, uint8_t posicao);
tam_t mem_livre();
tam_t mem_tamanho();
#endif
