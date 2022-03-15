/*
 * Interface com o Usuario via Texto
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - Versão inicial
 */

#ifndef IUT_H
#define IUT_H
#include "husis.h"
typedef unsigned short iut_id_t;
typedef unsigned short iut_opcoes_t;

void iut_iniciar(); 
void iut_desenha_caixa(tam_t x1, tam_t y1, tam_t largura, tam_t altura, cor_t cor_borda, cor_t cor_fundo);
void iut_desenha_linha_h(tam_t x1, tam_t y1, tam_t largura, cor_t cor_borda, cor_t cor_fundo);
void iut_desenha_rotulo(tam_t x1, tam_t y1, tam_t largura, txt_t txt, cor_t cor_texto, cor_t cor_fundo);
void iut_desenha_janela(tam_t x1, tam_t y1, tam_t largura, tam_t altura, txt_t titulo, iut_opcoes_t botoes);
void iut_desenha_progresso(tam_t x1, tam_t y1, tam_t largura, uint8_t percentual, cor_t cor_barra, cor_t cor_fundo);
tam_t iut_largura();
tam_t iut_altura();

#define IUT_BOTAO_NENHUM 0
#define IUT_BOTAO_FECHAR 1
#endif
