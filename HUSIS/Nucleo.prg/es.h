/*
 * Cabecalho do Modulo de Entrada e Saida Nativo
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - Versão inicial
 */

#ifndef ES_H
#define ES_H
#include "husis.h"

void husis(txt_t args);

void es_escreva_8(uint16_t porta, uint8_t valor);
uint8_t es_leia_8(uint16_t porta);
void es_escreva_16(uint16_t porta, uint16_t valor);
uint16_t es_leia_16(uint16_t porta);

#define ES_VIDEO_LIMITE_LINHA 1
#define ES_VIDEO_LIMITE_TELA 2

void es_video_iniciar();
void es_video_limpar(cor_t cor_texto, cor_t cor_fundo);
void es_video_escreva_c(tam_t x, tam_t y, uint8_t valor, cor_t cor_texto, cor_t cor_fundo);
void es_video_escreva_c_repetido(tam_t x, tam_t y, tam_t qtd, uint8_t c, cor_t cor_texto, cor_t cor_fundo, uint8_t limite);
void es_video_escreva_txt(tam_t x, tam_t y, tam_t tam, txt_t txt, cor_t cor_texto, cor_t cor_fundo, uint8_t limite);
tam_t es_video_largura();
tam_t es_video_altura();
void es_video_oculta_cursor();
void es_video_exiba_cursor();
void es_video_atualiza_cursor();

 
#endif
