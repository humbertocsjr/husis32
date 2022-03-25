#ifndef PROCESSO_H
#define PROCESSO_H
#include "husis.h"

#define PROCESSO_TOTAL 256

#define PROCESSO_VAZIO 0
#define PROCESSO_RESERVADO 255
#define PROCESSO_NUCLEO 1

#define PROCESSO_STATUS_VAZIO 0
#define PROCESSO_STATUS_EXECUTANDO 1
#define PROCESSO_STATUS_AGUARDANDO 2
#define PROCESSO_STATUS_SUSPENSO 3
#define PROCESSO_STATUS_INVALIDO 4

struct processo_dados
{
    uint8_t status;
    posicao_t seletor;
    posicao_t pilha;
    txt_t nome;
};

typedef struct processo_dados processo_dados_t;

processo_t processo_atual();
processo_t processo_proximo_processo();
void processo_prepara();
status_t processo_valido(processo_t processo);
status_t processo_acessivel(processo_t processo);
status_t processo_altera_nome(processo_t processo, txt_t nome);
status_t processo_leia_nome(processo_t processo, txt_t destino, tam_t capacidade);
status_t processo_altera_status(processo_t processo, posicao_t status_processo);
status_t processo_leia_status(processo_t processo, posicao_t * status_processo);

#endif
