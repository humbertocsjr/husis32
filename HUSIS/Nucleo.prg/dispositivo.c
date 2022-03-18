/*
 * Gestor de Dispositivos
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.17 - Versão inicial
 */ 

#include "husis.h"
#include "txt.h"
#include "dispositivo.h"

dispositivo_t _dispositivos[DISPOSITIVO_TOTAL];


// DISPOSITIVOS DE DISCO

#include "disquete.h"
#include "ide.h"

void dispositivo_disco_inicia()
{
    disquete_inicia();
    ide_inicia();
}

// GESTOR DE DISPOSITIVOS

void dispositivo_inicia()
{
    for(posicao_t i = 0; i < DISPOSITIVO_TOTAL; i ++)
    {
        _dispositivos[i].tipo = DISPOSITIVO_TIPO_INDEFINIDO;
        _dispositivos[i].nome[0] = 0;
    }
}

tam_t dispositivo_quantidade()
{
    tam_t qtd = 0;
    for(posicao_t i =0; i < DISPOSITIVO_TOTAL; i++)
    {
        if(_dispositivos[i].tipo != DISPOSITIVO_TIPO_INDEFINIDO) qtd++;
    }
    return qtd;
}

posicao_t dispositivo_registra(txt_t nome, uint8_t tipo, tam_t tamanho_bloco)
{
    for(posicao_t i = 1; i < DISPOSITIVO_TOTAL; i ++)
    {
        if(_dispositivos[i].tipo == DISPOSITIVO_TIPO_INDEFINIDO)
        {
            _dispositivos[i].tipo = tipo;
            txt_copia(nome, _dispositivos[i].nome, DISPOSITIVO_TAM_NOME);
            _dispositivos[i].tamanho_bloco = tamanho_bloco;
            _dispositivos[i].leia_bloco = 0;
            _dispositivos[i].grava_bloco = 0;
            return i;
        }
    }
    return 0;
}

posicao_t dispositivo_busca(txt_t nome)
{
    for(posicao_t i = 1; i < DISPOSITIVO_TOTAL; i++)
    {
        if(_dispositivos[i].tipo != DISPOSITIVO_TIPO_INDEFINIDO)
        {
            if(txt_igual(nome, _dispositivos[i].nome, DISPOSITIVO_TAM_NOME) == OK)
            {
                return i;
            }
        }
    }
    return 0;
}

status_t dispositivo_status(posicao_t dispositivo)
{
    if(dispositivo == DISPOSITIVO_NAO_ENCONTRADO) return ERRO_NAO_ENCONTRADO;
    if(_dispositivos[dispositivo].tipo != DISPOSITIVO_TIPO_INDEFINIDO) return OK;
    return ERRO_NAO_ENCONTRADO;
}

status_t dispositivo_altera_leia(posicao_t dispositivo, tam_t (* leia_bloco)(byte_t * destino, posicao_t posicao, tam_t quantidade))
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    _dispositivos[dispositivo].leia_bloco = leia_bloco;
}

status_t dispositivo_altera_grava(posicao_t dispositivo, tam_t (* grava_bloco)(byte_t * destino, posicao_t posicao, tam_t quantidade))
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    _dispositivos[dispositivo].grava_bloco = grava_bloco;
}

status_t dispositivo_altera_tamanho(posicao_t dispositivo, tam_t tamanho)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    _dispositivos[dispositivo].tamanho = tamanho;
}

status_t dispositivo_altera_config(posicao_t dispositivo, posicao_t posicao, posicao_t valor)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    if(posicao >= DISPOSITIVO_TAM_CONFIG) return ERRO_ESTOURO_DE_CAPACIDADE;
    _dispositivos[dispositivo].config[32] = valor;
}

status_t dispositivo_leia_config(posicao_t dispositivo, posicao_t posicao, posicao_t * destino)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    if(posicao >= DISPOSITIVO_TAM_CONFIG) return ERRO_ESTOURO_DE_CAPACIDADE;
    destino = _dispositivos[dispositivo].config[32];
}

status_t dispositivo_tamanho(posicao_t dispositivo, tam_t * destino)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    destino = _dispositivos[dispositivo].tamanho;
}

tam_t dispositivo_leia_bloco(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    if((posicao_t)_dispositivos[dispositivo].leia_bloco == 0) return ERRO_NAO_ENCONTRADO;
    return _dispositivos[dispositivo].leia_bloco(destino, posicao, quantidade);
}

tam_t dispositivo_grava_bloco(posicao_t dispositivo, byte_t * origem, posicao_t posicao, tam_t quantidade)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    if((posicao_t)_dispositivos[dispositivo].grava_bloco == 0) return ERRO_NAO_ENCONTRADO;
    return _dispositivos[dispositivo].grava_bloco(origem, posicao, quantidade);
}

status_t dispositivo_remove(posicao_t dispositivo)
{
    if(dispositivo_status(dispositivo) != OK) return ERRO_NAO_ENCONTRADO;
    _dispositivos[dispositivo].tipo = DISPOSITIVO_TIPO_INDEFINIDO;
    _dispositivos[dispositivo].nome[0] = 0;
}



