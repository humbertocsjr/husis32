#include "iut.h"
#include "es.h"

void iut_iniciar()
{
    es_video_limpar(COR_BRANCO, COR_PRETO);
    es_video_oculta_cursor();
}

void iut_desenha_caixa(tam_t x1, tam_t y1, tam_t largura, tam_t altura, cor_t cor_borda, cor_t cor_fundo)
{
    es_video_escreva_c(x1, y1, 218, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1, 191, cor_borda, cor_fundo);
    
    for(tam_t y = y1 + 1; y < (y1 + altura - 1); y++)
    {
        es_video_escreva_c(x1, y, 179, cor_borda, cor_fundo);
        es_video_escreva_c_repetido(x1 + 1, y, largura - 2, ' ', cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
        es_video_escreva_c(x1 + largura - 1, y, 179, cor_borda, cor_fundo);
    }
    
    es_video_escreva_c(x1, y1 + altura -1, 192, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1 + altura -1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1 + altura -1, 217, cor_borda, cor_fundo);
}

void iut_desenha_linha_h(tam_t x1, tam_t y1, tam_t largura, cor_t cor_borda, cor_t cor_fundo)
{
    es_video_escreva_c(x1, y1, 195, cor_borda, cor_fundo);
    es_video_escreva_c_repetido(x1 + 1, y1, largura - 2, 196, cor_borda, cor_fundo, ES_VIDEO_LIMITE_LINHA);
    es_video_escreva_c(x1 + largura - 1, y1, 180, cor_borda, cor_fundo);
}

void iut_desenha_rotulo(tam_t x1, tam_t y1, tam_t largura, txt_t txt, cor_t cor_texto, cor_t cor_fundo)
{
    es_video_escreva_txt(x1, y1, largura, txt, cor_texto, cor_fundo, ES_VIDEO_LIMITE_LINHA);
}

void iut_desenha_janela(tam_t x1, tam_t y1, tam_t largura, tam_t altura, txt_t titulo, iut_opcoes_t botoes)
{
    iut_desenha_caixa(x1, y1, largura, altura, COR_CIANO, COR_PRETO);
    iut_desenha_linha_h(x1, y1 + 2, largura, COR_CIANO, COR_PRETO);
    iut_desenha_rotulo(x1 + 2, y1 + 1, largura - 5, titulo, COR_CIANO_CLARO, COR_PRETO);
    iut_desenha_rotulo(x1 + largura - 3, y1 + 1, 2, "x", COR_VERMELHO_CLARO, COR_PRETO);
}

void iut_desenha_progresso(tam_t x1, tam_t y1, tam_t largura, uint8_t percentual, cor_t cor_barra, cor_t cor_fundo)
{
    if(percentual > 100) percentual = 100;
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
