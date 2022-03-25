#include "../husis.h"
#include "../dispositivo.h"
#include "../es_video.h"
#include "../es_int.h"

// Memoria Temporaria Circular para armazenar as teclas apertadas.

#define TECLADO_MAX_TEMP 32

uint8_t _teclado_temp[TECLADO_MAX_TEMP];
uint8_t _teclado_pos_leitura = 0;
uint8_t _teclado_pos_gravacao = 0;
uint8_t _teclado_tamanho = 0;
uint8_t _teclado_ultima_tecla = 0;

posicao_t _teclado = 0;

void teclado_manipulador(posicao_t reg1, posicao_t reg2, posicao_t reg3, posicao_t reg4, posicao_t interrupcao, posicao_t codigo_erro)
{
    // Teclado
    uint8_t tecla = es_leia_8(0x60);
    
    if(tecla & 0x80)
    {
        
    }
    else
    {
        es_video_escreva_nro(30,0,10, tecla, COR_PRETO, COR_BRANCO);
        es_video_escreva_nro(40,0,10, _teclado_tamanho, COR_PRETO, COR_BRANCO);
    }
    
    if(!(_teclado_pos_gravacao != _teclado_pos_leitura | _teclado_tamanho == 0) | _teclado_ultima_tecla == tecla)
    {
        return;
    }
    
    _teclado_temp[_teclado_pos_gravacao] = tecla;
    _teclado_pos_gravacao = (_teclado_pos_gravacao + 1) % TECLADO_MAX_TEMP;
    _teclado_tamanho ++;
    _teclado_ultima_tecla = tecla;
    
    dispositivo_altera_tamanho(_teclado, _teclado_tamanho);
    
    
}

tam_t teclado_leia_bloco(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade)
{
    posicao_t i = 0;
    for(i = 0; i < quantidade; i++)
    {
        if(_teclado_tamanho == 0) break;
        destino[posicao + i] = _teclado_temp[_teclado_pos_leitura];
        _teclado_temp[_teclado_pos_leitura] = 0;
        _teclado_tamanho--;
        _teclado_ultima_tecla = 0;
        _teclado_pos_leitura = (_teclado_pos_leitura + 1) % TECLADO_MAX_TEMP;
        dispositivo_altera_tamanho(_teclado, _teclado_tamanho);
    }
    return i;
}

status_t teclado_inicia()
{
    status_t ret = OK;
    
    for(posicao_t i = 0; i < TECLADO_MAX_TEMP; i++)
    {
        _teclado_temp[i] = 0;
    }
    
    
    _teclado = dispositivo_registra("Teclado", DISPOSITIVO_TIPO_TECLADO, 1);
    VALIDA(dispositivo_status(_teclado));
    VALIDA(dispositivo_altera_leia(_teclado, &teclado_leia_bloco));
    VALIDA(dispositivo_altera_tamanho(_teclado, _teclado_tamanho));
    
    VALIDA(es_int_altera(33, &teclado_manipulador));
    return ret;
}
