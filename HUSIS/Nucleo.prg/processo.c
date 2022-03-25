#include "husis.h"
#include "processo.h" 
#include "es.h"

processo_dados_t _processos[PROCESSO_TOTAL];

processo_t _processo_atual = PROCESSO_NUCLEO;

processo_t processo_atual()
{
    return _processo_atual;
}

processo_t processo_proximo_processo()
{
    for(processo_t i = _processo_atual + 1; i <= PROCESSO_TOTAL; i++)
    {
        if(i == PROCESSO_TOTAL) i = 0;
        if(_processos[i].status == PROCESSO_STATUS_EXECUTANDO)
        {
            return i;
        }
    }
    return PROCESSO_NUCLEO;
}

void processo_prepara()
{
    for(processo_t i = 0; i < PROCESSO_TOTAL; i++)
    {
        _processos[i].status = PROCESSO_STATUS_VAZIO;
        _processos[i].seletor = 0;
        _processos[i].pilha = 0;
    }
    _processos[0].status = PROCESSO_STATUS_INVALIDO;
    
}

status_t processo_valido(processo_t processo)
{
    if(processo >= PROCESSO_TOTAL) return ERRO_ESTOURO_DE_CAPACIDADE;
    if(_processos[processo].status == PROCESSO_STATUS_VAZIO) return ERRO_NAO_ENCONTRADO;
    return OK;
}

status_t processo_acessivel(processo_t processo)
{
    status_t ret = OK;
    if(processo == PROCESSO_NUCLEO) return ret;
    VALIDA(processo_valido(processo));
    if(processo != _processo_atual) return ERRO_ACESSO_NEGADO;
    return ret;
}

status_t processo_altera_nome(processo_t processo, txt_t nome)
{
    status_t ret = OK;
    VALIDA(processo_acessivel(processo));
    _processos[processo].nome = nome;
    return ret;
}

status_t processo_leia_nome(processo_t processo, txt_t destino, tam_t capacidade)
{
    status_t ret = OK;
    VALIDA(processo_valido(processo));
    txt_copia(_processos[processo].nome, destino, capacidade);
    return ret;
}

status_t processo_altera_status(processo_t processo, posicao_t status_processo)
{
    status_t ret = OK;
    VALIDA(processo_acessivel(processo));
    _processos[processo].status = status_processo;
    return ret;
}

status_t processo_leia_status(processo_t processo, posicao_t * status_processo)
{
    status_t ret = OK;
    VALIDA(processo_valido(processo));
    status_processo = _processos[processo].status;
    return ret;
}
