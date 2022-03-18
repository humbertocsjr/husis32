/*
 * Cabecalho do Modulo de Entrada e Saida Nativo
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.17 - Incluido retorno do tamanho do rotulo escrito e rotulo numerico
 */

#ifndef ES_VIDEO_H
#define ES_VIDEO_H
#include "husis.h"

#define ES_VIDEO_LIMITE_LINHA 1
#define ES_VIDEO_LIMITE_TELA 2

void es_video_inicia();
void es_video_limpa(cor_t cor_texto, cor_t cor_fundo);
void es_video_escreva_c(tam_t x, tam_t y, uint8_t valor, cor_t cor_texto, cor_t cor_fundo);
void es_video_escreva_c_repetido(tam_t x, tam_t y, tam_t qtd, uint8_t c, cor_t cor_texto, cor_t cor_fundo, uint8_t limite);
tam_t es_video_escreva_txt(tam_t x, tam_t y, tam_t tam, txt_t txt, cor_t cor_texto, cor_t cor_fundo, uint8_t limite);
tam_t es_video_escreva_nro(tam_t x, tam_t y, tam_t tam, uint32_t nro, cor_t cor_texto, cor_t cor_fundo);
tam_t es_video_largura();
tam_t es_video_altura();
void es_video_oculta_cursor();
void es_video_exiba_cursor();
void es_video_atualiza_cursor();

#endif
 
