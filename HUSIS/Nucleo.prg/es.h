/*
 * Cabecalho do Modulo de Entrada e Saida Nativo
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 */

#ifndef ES_H
#define ES_H
#include "husis.h"

void husis(txt_t args);

void es_escreva_8(uint16_t porta, uint8_t valor);
uint8_t es_leia_8(uint16_t porta);
void es_escreva_16(uint16_t porta, uint16_t valor);
uint16_t es_leia_16(uint16_t porta);

posicao_t es_nucleo_primeira_pagina();
posicao_t es_nucleo_ultima_pagina();
void es_aplicar_areas_restritas(tam_t capacidade_paginas);
tam_t es_mem_tamanho();

#endif
