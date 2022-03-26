#include "husis.h"
#include "agenda.h"
#include "config.h"


agendamento_t * _agenda[AGENDA_MAX];

void agenda_inicia()
{
    for(posicao_t i = 0; i < AGENDA_MAX; i++)
    {
        _agenda[i] = 0;
    }
}

status_t agenda_adiciona(agendamento_t * agendamento)
{
    status_t ret = OK;
    for(posicao_t i = 0; i < AGENDA_MAX; i++)
    {
        if(_agenda[i] == 0)
        {
            agendamento->proximo = es_contador_decimos() + es_converter_decimos(agendamento->decimos);
            _agenda[i] = agendamento;
        }
    }
    return ret;
}

 
