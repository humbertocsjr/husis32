#include "husis.h"
#include "versao.h"
#include "es.h"
#include "iut.h"

void husis_progresso(txt_t texto1, txt_t texto2, uint8_t percentual)
{
    iut_desenha_janela(10, 5, iut_largura() - 20, iut_altura() - 10, husis_nome, IUT_BOTAO_FECHAR);
    iut_desenha_rotulo(12, 8, iut_largura() - 24, texto1, COR_CIANO, COR_PRETO);
    iut_desenha_rotulo(12, 9, iut_largura() - 24, texto2, COR_CIANO, COR_PRETO);
    iut_desenha_rotulo(12, iut_altura() - 8, iut_largura() - 24, "Progresso: ", COR_CIANO, COR_PRETO);
    iut_desenha_progresso(12, iut_altura() - 7, iut_largura() - 24, percentual, COR_CIANO, COR_PRETO);
}

void husis(txt_t args)
{
    es_video_iniciar(COR_CIANO, COR_PRETO);
    iut_iniciar();
    husis_progresso("Iniciando . . .", "", 10);
}
