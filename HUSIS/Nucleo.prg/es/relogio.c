#include "../husis.h"
#include "../dispositivo.h"
#include "../es.h"
#include "../es_int.h"
#include "../agenda.h"

#ifndef SEM_INTERFACE
#include "../iut.h"
#endif

posicao_t _relogio_ano_inicial = 2000;
data_t _relogio_data;
agendamento_t _relogio_agendamento;

void relogio_agenda(agendamento_t * agendamento, posicao_t * valor, posicao_t contador)
{
    uint8_t temp;
    
    es_int_para();
    es_escreva_8(0x70, 0xa);
    temp = es_leia_8(0x71) & 0x80;
    
    if(temp == 0)
    {
        es_escreva_8(0x70, 0x9);
        _relogio_data.ano = es_leia_8(0x71);
        es_escreva_8(0x70, 0x8);
        _relogio_data.mes = es_leia_8(0x71);
        es_escreva_8(0x70, 0x7);
        _relogio_data.dia = es_leia_8(0x71);
        es_escreva_8(0x70, 0x4);
        _relogio_data.hora = es_leia_8(0x71);
        es_escreva_8(0x70, 0x2);
        _relogio_data.min = es_leia_8(0x71);
        es_escreva_8(0x70, 0x0);
        _relogio_data.seg = es_leia_8(0x71);
        
        es_escreva_8(0x70, 0xb);
        if(!(es_leia_8(0x71) & 0x4))
        {
            _relogio_data.seg = (_relogio_data.seg & 0xf) + ((_relogio_data.seg / 16) * 10);
            _relogio_data.min = (_relogio_data.min & 0xf) + ((_relogio_data.min / 16) * 10);
            _relogio_data.hora = ((_relogio_data.hora & 0xf) + (((_relogio_data.hora & 0x70) / 16) * 10)) | (_relogio_data.hora & 0x80);
            _relogio_data.dia = (_relogio_data.dia & 0xf) + ((_relogio_data.dia / 16) * 10);
            _relogio_data.mes = (_relogio_data.mes & 0xf) + ((_relogio_data.mes / 16) * 10);
            _relogio_data.ano = (_relogio_data.ano & 0xf) + ((_relogio_data.ano / 16) * 10);
        }
        _relogio_data.ano += _relogio_ano_inicial;
#ifndef SEM_INTERFACE
        if(_relogio_agendamento.valor != _relogio_data.seg) iut_processa_data(_relogio_data);
#endif
        _relogio_agendamento.valor = _relogio_data.seg;
    }
    
    es_int_continua();
    
    agenda_adiciona(agendamento);
}

status_t relogio_inicia()
{
    _relogio_agendamento.decimos = 5;
    _relogio_agendamento.acao = &relogio_agenda;
    _relogio_agendamento.valor = 999;
    _relogio_data.seg = 0;
    agenda_adiciona(&_relogio_agendamento);
}
