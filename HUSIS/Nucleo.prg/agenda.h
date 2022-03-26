#ifndef AGENDA_H
#define AGENDA_H
#include "husis.h"

struct agendamento
{
    // Qual a proxima execucao em aproximadamente decimos de segundo
    posicao_t decimos;
    // Ação executada a cada passada
    void (* acao)(struct agendamento * agendamento, posicao_t * valor, posicao_t contador);
    // Parametro passado para a ação
    posicao_t valor;
    // Contador mantido pelo sistema operacional
    posicao_t contador;
    // Uso do sistema operacional
    posicao_t proximo;
};

typedef struct agendamento agendamento_t;

void agenda_inicia();

// Agenda uma acao para X decimos de segundo, caso seja recorrente readicionar, as ações devem ser de processamento rápido e exclusivas do núcleo
status_t agenda_adiciona(agendamento_t * agendamento);

#endif
