/*
 * Interface com o Usuario via Texto
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.17 - Incluido retorno do tamanho do rotulo escrito e rotulo numerico
 *              Implementado controles e gestao dos controles
 * 
 * Informações:
 * - Implementa os componentes visuais usando caracteres ASCII (Limitando ao CP850)
 * 
 */

#include "iut.h"
#include "es.h"
#include "es_video.h"

iut_controle_t _iut_tela;

cor_t _iut_estilo_tela_fundo = COR_PRETO;
cor_t _iut_estilo_borda_janela = COR_CIANO;
cor_t _iut_estilo_titulo_janela = COR_CIANO_CLARO;
cor_t _iut_estilo_fundo = COR_PRETO;
cor_t _iut_estilo_comum = COR_CIANO;
cor_t _iut_estilo_destaque = COR_CIANO_CLARO;
cor_t _iut_estilo_menu_comum = COR_PRETO;
cor_t _iut_estilo_menu_fundo = COR_CIANO;

uint8_t _iut_exibe_menu_principal = 0;

void iut_inicia()
{
    es_video_limpa(_iut_estilo_fundo, _iut_estilo_fundo);
    es_video_oculta_cursor();
    iut_zera_controle(&_iut_tela);
    _iut_tela.tipo = IUT_TIPO_TELA;
    _iut_tela.x = 0;
    _iut_tela.y = 0;
    _iut_tela.x_calc = 0;
    _iut_tela.y_calc = 0;
    _iut_tela.x1_conteudo = 0;
    _iut_tela.y1_conteudo = 1;
    _iut_tela.x2_conteudo = 0;
    _iut_tela.y2_conteudo = 0;
    _iut_tela.largura = es_video_largura();
    _iut_tela.altura = es_video_altura();
    _iut_tela.largura_calc = es_video_largura();
    _iut_tela.altura_calc = es_video_altura();
    _iut_tela.organizacao = IUT_ORGANIZACAO_FIXA;
}

void iut_redesenha_tela()
{
    iut_redesenha(&_iut_tela);
}

void iut_exibe_menu_principal()
{
    _iut_exibe_menu_principal = 1;
    iut_redesenha_tela();
}

void iut_oculta_menu_principal()
{
    _iut_exibe_menu_principal = 0;
    iut_redesenha_tela();
}

status_t iut_adiciona(iut_controle_t * acima, iut_controle_t * controle)
{
    for(posicao_t i = 0; i < IUT_MAX_CONTEUDO; i++)
    {
        if(acima->conteudo[i] == 0)
        {
            acima->conteudo[i] = controle;
            controle->acima = acima;
            return OK;
        }
    }
    return ERRO_ESTOURO_DE_CAPACIDADE;
}

void iut_altera_texto(iut_controle_t * controle, txt_t texto)
{
    controle->texto = texto;
    iut_redesenha(controle);
}

void iut_altera_valor(iut_controle_t * controle, posicao_t valor)
{
    controle->valor = valor;
    iut_redesenha(controle);
}

void iut_altera_posicao(iut_controle_t * controle, posicao_t x, posicao_t y)
{
    controle->x = x;
    controle->y = y;
    iut_redesenha(controle);
}

void iut_exibe(iut_controle_t * controle)
{
    controle->exibicao = IUT_EXIBICAO_VISIVEL;
    iut_redesenha(controle);
}

void iut_oculta(iut_controle_t * controle)
{
    controle->exibicao = IUT_EXIBICAO_OCULTO;
    if((posicao_t)controle->acima != 0) iut_redesenha(controle->acima);
}

void iut_nova_linha(iut_controle_t * controle)
{
    controle->posicao_dinamica = IUT_POSICAO_DINAMICA_ABAIXO;
}

void iut_redesenha(iut_controle_t * controle)
{
    if(controle == &_iut_tela)
    {
        es_video_limpa(_iut_estilo_tela_fundo, _iut_estilo_tela_fundo);
        iut_redesenha_menus(0);
        iut_redesenha_conteudo(controle);
    }
    
    iut_controle_t * janela = 0;
    if((posicao_t)controle->acima != 0)
    {
        janela = controle->acima;
        // Busca a janela ou o componente mais acima oculto para impedir a renderizacao
        while((janela->tipo != IUT_TIPO_JANELA && (posicao_t)janela->acima != 0))
        {
            if(janela->exibicao == IUT_EXIBICAO_OCULTO) break;
            janela = janela->acima;
        }
        // Calcula a posicao real na tela baseado no item acima
        controle->x_calc = controle->acima->x_calc + controle->acima->x1_conteudo + controle->x;
        controle->y_calc = controle->acima->y_calc + controle->acima->y1_conteudo + controle->y;
        controle->largura_calc = controle->acima->largura_calc - controle->acima->x2_conteudo - controle->x - controle->acima->x1_conteudo;
        controle->altura_calc = controle->acima->altura_calc - controle->acima->y2_conteudo - controle->y - controle->acima->y1_conteudo;
        if(controle->largura_calc > controle->largura) controle->largura_calc = controle->largura;
        if(controle->altura_calc > controle->altura) controle->altura_calc = controle->altura;
    }
    if(
        (posicao_t)controle->redesenha != 0 && 
        controle->largura_calc > 0 && 
        controle->altura_calc > 0 &&
        controle->exibicao == IUT_EXIBICAO_VISIVEL &&
        (
            janela != 0 && janela->exibicao == IUT_EXIBICAO_VISIVEL
        )
    )
        if(
            controle->x_calc <= (controle->acima->largura_calc - controle->acima->x2_conteudo + controle->acima->x_calc) &&
            controle->y_calc <= (controle->acima->altura_calc - controle->acima->y2_conteudo + controle->acima->y_calc)
        )
            controle->redesenha(controle);
}

void iut_redesenha_conteudo(iut_controle_t * controle)
{
    for(posicao_t i = 0; i < IUT_MAX_CONTEUDO; i++)
    {
        if(controle->conteudo[i] != 0)
        {
            iut_redesenha(controle->conteudo[i]);
        }
    }
}

void iut_redesenha_menus(iut_controle_t * controle)
{
    es_video_escreva_c_repetido(0,0, es_video_largura(), ' ', _iut_estilo_menu_comum, _iut_estilo_menu_fundo, ES_VIDEO_LIMITE_LINHA);
    if(_iut_exibe_menu_principal != 0)
    {
        es_video_escreva_c(1,0, 4, _iut_estilo_menu_comum, _iut_estilo_menu_fundo);
    }
}

void iut_redesenha_janela(iut_controle_t * controle)
{
    iut_desenha_janela(controle->x_calc, controle->y_calc, controle->largura_calc, controle->altura_calc, controle->texto, controle->opcoes);
    iut_redesenha_conteudo(controle);
    iut_redesenha_menus((iut_controle_t *) controle->valor);
}

void iut_redesenha_rotulo(iut_controle_t * controle)
{
    iut_desenha_rotulo(controle->x_calc, controle->y_calc, controle->largura_calc, controle->texto, _iut_estilo_comum, _iut_estilo_fundo);
}

void iut_redesenha_rotulo_nro(iut_controle_t * controle)
{
    iut_desenha_rotulo_nro(controle->x_calc, controle->y_calc, controle->largura_calc, controle->valor, _iut_estilo_comum, _iut_estilo_fundo);
}

void iut_redesenha_progresso(iut_controle_t * controle)
{
    iut_desenha_progresso(controle->x_calc, controle->y_calc, controle->largura_calc, controle->valor, _iut_estilo_comum, _iut_estilo_fundo);
}

status_t iut_nova_janela(iut_controle_t * controle, posicao_t x, posicao_t y, posicao_t largura, posicao_t altura, txt_t titulo, iut_opcoes_t botoes)
{
    iut_zera_controle(controle);
    controle->tipo = IUT_TIPO_JANELA;
    controle->x = x;
    controle->y = y;
    controle->altura = altura;
    controle->largura = largura;
    controle->x1_conteudo = 2;
    controle->y1_conteudo = 3;
    controle->x2_conteudo = 2;
    controle->y2_conteudo = 1;
    controle->texto = titulo;
    controle->organizacao = IUT_ORGANIZACAO_FIXA;
    controle->redesenha = iut_redesenha_janela;
    controle->opcoes = botoes;
    controle->exibicao = IUT_EXIBICAO_OCULTO;
    controle->valor = 0;
    return iut_adiciona(&_iut_tela, controle);
}

void iut_novo_rotulo(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, txt_t texto)
{
    iut_zera_controle(controle);
    controle->tipo = IUT_TIPO_ROTULO;
    controle->x = x;
    controle->y = y;
    controle->altura = 1;
    controle->largura = largura;
    controle->texto = texto;
    controle->redesenha = iut_redesenha_rotulo;
}

void iut_novo_rotulo_nro(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, posicao_t valor)
{
    iut_zera_controle(controle);
    controle->tipo = IUT_TIPO_ROTULO_NRO;
    controle->x = x;
    controle->y = y;
    controle->altura = 1;
    controle->largura = largura;
    controle->valor = valor;
    controle->redesenha = iut_redesenha_rotulo_nro;
}

void iut_novo_progresso(iut_controle_t * controle, posicao_t x, posicao_t y, tam_t largura, posicao_t valor)
{
    iut_zera_controle(controle);
    controle->tipo = IUT_TIPO_PROGRESSO;
    controle->x = x;
    controle->y = y;
    controle->altura = 1;
    controle->largura = largura;
    controle->valor = valor;
    controle->redesenha = iut_redesenha_progresso;
}

void iut_zera_controle(iut_controle_t * controle)
{
    controle->acima = 0;
    controle->organizacao = IUT_ORGANIZACAO_FIXA;
    controle->posicao_dinamica = IUT_POSICAO_DINAMICA_CONTINUA;
    for(posicao_t i = 0; i < IUT_MAX_CONTEUDO; i++)
    {
        controle->conteudo[i] = 0;
    }
    controle->redesenha = 0;
    controle->x_calc = 0;
    controle->y_calc = 0;
    controle->x1_conteudo = 0;
    controle->y1_conteudo = 0;
    controle->x2_conteudo = 0;
    controle->y2_conteudo = 0;
    controle->largura_calc = 0;
    controle->altura_calc = 0;
    controle->texto = (txt_t *)0;
    controle->valor = 0;
    controle->exibicao = IUT_EXIBICAO_VISIVEL;
}

void iut_limpa_tela(cor_t cor_fundo)
{
    es_video_limpa(COR_BRANCO, cor_fundo);
}

void iut_desenha_caixa(tam_t x1, tam_t y1, tam_t largura, tam_t altura, cor_t cor_borda, cor_t cor_fundo)
{
    // Desenha a parte superior da caixa
    es_video_escreva_c(x1, y1, 218, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1, 191, cor_borda, cor_fundo);
    
    // Desenha o meio da caixa
    for(tam_t y = y1 + 1; y < (y1 + altura - 1); y++)
    {
        es_video_escreva_c(x1, y, 179, cor_borda, cor_fundo);
        es_video_escreva_c_repetido(x1 + 1, y, largura - 2, ' ', cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
        es_video_escreva_c(x1 + largura - 1, y, 179, cor_borda, cor_fundo);
    }
    
    // Desenha a parte inferior da caixa
    es_video_escreva_c(x1, y1 + altura -1, 192, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1 + altura -1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1 + altura -1, 217, cor_borda, cor_fundo);
}

void iut_desenha_linha_h(tam_t x1, tam_t y1, tam_t largura, cor_t cor_borda, cor_t cor_fundo)
{
    // Desenha uma linha horizontal na tela
    es_video_escreva_c(x1, y1, 195, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1, 180, cor_borda, cor_fundo);
}

tam_t iut_desenha_rotulo(tam_t x1, tam_t y1, tam_t largura, txt_t txt, cor_t cor_texto, cor_t cor_fundo)
{
    // Desenha um rotulo de texto
    return es_video_escreva_txt(x1, y1, largura, txt, cor_texto, cor_fundo, ES_VIDEO_LIMITE_LINHA);
}

tam_t iut_desenha_rotulo_nro(tam_t x1, tam_t y1, tam_t largura, uint32_t nro, cor_t cor_texto, cor_t cor_fundo)
{
    // Desenha um rotulo de texto
    return es_video_escreva_nro(x1, y1, largura, nro, cor_texto, cor_fundo);
}

void iut_desenha_janela(tam_t x1, tam_t y1, tam_t largura, tam_t altura, txt_t titulo, iut_opcoes_t botoes)
{
    // Desenha uma janela usando os componentes comuns
    iut_desenha_caixa(x1, y1, largura, altura, _iut_estilo_borda_janela, _iut_estilo_fundo);
    iut_desenha_linha_h(x1, y1 + 2, largura, _iut_estilo_borda_janela, _iut_estilo_fundo);
    iut_desenha_rotulo(x1 + 2, y1 + 1, largura - 5, titulo, _iut_estilo_titulo_janela, _iut_estilo_fundo);
    if(botoes == IUT_BOTAO_FECHAR)
    {
        iut_desenha_rotulo(x1 + largura - 3, y1 + 1, 2, "x", COR_VERMELHO_CLARO, _iut_estilo_fundo);
    }
}

void iut_desenha_progresso(tam_t x1, tam_t y1, tam_t largura, uint8_t percentual, cor_t cor_barra, cor_t cor_fundo)
{
    if(percentual > 100) percentual = 100;
    // Desenha uma barra de progresso usando caracteres claros e escuros
    es_video_escreva_c_repetido(x1, y1, largura , 176, cor_barra, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c_repetido(x1, y1, largura * percentual / 100, 178, cor_barra, cor_fundo, ES_VIDEO_LIMITE_LINHA);
}

tam_t iut_largura()
{
    return es_video_largura();
}

tam_t iut_altura()
{
    return es_video_altura();
}
