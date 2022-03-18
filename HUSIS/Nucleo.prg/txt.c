
#include "husis.h"
#include "txt.h"

tam_t txt_tam(txt_t texto, tam_t capacidade)
{
    tam_t tam = 0;
    for(tam = 0; tam < capacidade; tam++)
    {
        if(texto[tam] == 0) break;
    }
    return tam;
}

status_t txt_copia(txt_t origem, txt_t destino, tam_t capacidade)
{
    for(posicao_t i = 0; i < capacidade; i++)
    {
        destino[i] = origem[i];
        if(origem[i] == 0)
        {
            return OK;
        }
    }
    return ERRO_ESTOURO_DE_CAPACIDADE;
}

status_t txt_igual(txt_t txt1, txt_t txt2, tam_t capacidade)
{
    status_t retorno = IGUAL;
    for(posicao_t i = 0; i < capacidade; i++)
    {
        if(txt1[i] != txt2[i])
            retorno = DIFERENTE;
        if(txt1[i] == 0)
            break;
    }
    return retorno;
}

status_t txt_param(txt_t args, txt_t nome, txt_t valor, tam_t capacidade)
{
    
}
