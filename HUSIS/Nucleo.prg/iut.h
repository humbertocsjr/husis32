/*
 * Interface com o Usuario via Texto
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.17 - Incluido retorno do tamanho do rotulo escrito e rotulo numerico
 *              Implementado controles e gestao dos controles
 */

#ifndef IUT_H
#define IUT_H
#include "husis.h"
typedef unsigned short iut_id_t;
typedef unsigned short iut_opcoes_t;

#define IUT_MAX_CONTEUDO 64

struct iut_controle
{
    struct iut_controle * acima;
    posicao_t x;
    posicao_t y;
    tam_t largura;
    tam_t altura;
    posicao_t x_calc;
    posicao_t y_calc;
    tam_t largura_calc;
    tam_t altura_calc;
    posicao_t x1_conteudo;
    posicao_t y1_conteudo;
    tam_t x2_conteudo;
    tam_t y2_conteudo;
    iut_opcoes_t tipo;
    iut_opcoes_t organizacao;
    iut_opcoes_t posicao_dinamica;
    txt_t texto;
    uint32_t valor;
    iut_opcoes_t opcoes;
    iut_opcoes_t exibicao;
    void (*ao_entrar_em_foco)(struct iut_controle * controle);
    void (*ao_sair_de_foco)(struct iut_controle * controle);
    void (*ao_pressionar_tela)(struct iut_controle * controle);
    void (*ao_clicar)(struct iut_controle * controle);
    void (*ao_redimensionar)(struct iut_controle * controle);
    void (*redesenha)(struct iut_controle * controle);
    struct iut_controle * conteudo[IUT_MAX_CONTEUDO];
};

typedef struct iut_controle iut_controle_t;


struct iut_pacote
{
    posicao_t acao;
    iut_controle_t * controle;
    iut_controle_t * acima;
    txt_t texto;
    posicao_t valor;
    posicao_t x;
    posicao_t y;
    posicao_t largura;
    posicao_t altura;
    iut_opcoes_t opcoes;
};

typedef struct iut_pacote iut_pacote_t;

#define IUT_ACAO_ALTERA_VALOR 1
#define IUT_ACAO_ALTERA_TEXTO 2
#define IUT_ACAO_REDESENHA 3
#define IUT_ACAO_REDESENHA_TELA 4
#define IUT_ACAO_NOVA_JANELA 10
#define IUT_ACAO_NOVO_ROTULO 11
#define IUT_ACAO_NOVO_PROGRESSO 12

#define IUT_ORGANIZACAO_FIXA 0;
#define IUT_ORGANIZACAO_DINAMICA 1;

#define IUT_POSICAO_DINAMICA_CONTINUA 0
#define IUT_POSICAO_DINAMICA_ABAIXO 1

#define IUT_TIPO_TELA 0
#define IUT_TIPO_JANELA 1
#define IUT_TIPO_ROTULO 2
#define IUT_TIPO_ROTULO_NRO 3
#define IUT_TIPO_PROGRESSO 4

#define IUT_BOTAO_NENHUM 0
#define IUT_BOTAO_FECHAR 1
#define IUT_BOTAO_OK 2
#define IUT_BOTAO_CANCELAR 4

#define IUT_EXIBICAO_VISIVEL 0
#define IUT_EXIBICAO_OCULTO 1

void iut_inicia(); 

status_t iut_adiciona(iut_controle_t * acima, iut_controle_t * controle);
void iut_redesenha(iut_controle_t * controle);


void iut_altera_texto(iut_controle_t * controle, txt_t texto);
void iut_altera_valor(iut_controle_t * controle, posicao_t valor);
void iut_altera_posicao(iut_controle_t * controle, posicao_t x, posicao_t y);

void iut_exibe(iut_controle_t * controle);
void iut_oculta(iut_controle_t * controle);

status_t iut_nova_janela(iut_controle_t * controle, posicao_t x, posicao_t y, posicao_t largura, posicao_t altura, txt_t titulo, iut_opcoes_t botoes);
void iut_novo_rotulo(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, txt_t texto);
void iut_novo_rotulo_nro(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, posicao_t valor);
void iut_novo_progresso(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, posicao_t valor);

void iut_zera_controle(iut_controle_t * controle);

void iut_limpa_tela(cor_t cor_fundo);

void iut_desenha_caixa(tam_t x1, tam_t y1, tam_t largura, tam_t altura, cor_t cor_borda, cor_t cor_fundo);
void iut_desenha_linha_h(tam_t x1, tam_t y1, tam_t largura, cor_t cor_borda, cor_t cor_fundo);
tam_t iut_desenha_rotulo(tam_t x1, tam_t y1, tam_t largura, txt_t txt, cor_t cor_texto, cor_t cor_fundo);
tam_t iut_desenha_rotulo_nro(tam_t x1, tam_t y1, tam_t largura, uint32_t nro, cor_t cor_texto, cor_t cor_fundo);
void iut_desenha_janela(tam_t x1, tam_t y1, tam_t largura, tam_t altura, txt_t titulo, iut_opcoes_t botoes);
void iut_desenha_progresso(tam_t x1, tam_t y1, tam_t largura, uint8_t percentual, cor_t cor_barra, cor_t cor_fundo);

tam_t iut_largura();
tam_t iut_altura();
#endif
