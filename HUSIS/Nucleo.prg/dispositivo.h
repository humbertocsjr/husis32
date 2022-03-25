/*
 * Gestor de Dispositivos
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.17 - Versão inicial
 */ 

#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include "husis.h"

/*
 * Dentro do nucleo durante a inicializacao usar a macro DISPOSITIVO INICIA:
 * 
 *     DISPOSITIVO_INICIA(ide, "Erro ao carregar o Controlador IDE");
 * 
 * Não precisa declarar a função ide_inicia(), ela é declarada automaticamente.
 */

#define DISPOSITIVO_INICIA(disp, erro) extern status_t disp##_inicia(); if((ret = disp##_inicia()) != OK) husis_progresso_erro(erro, ret)



#define DISPOSITIVO_TAM_NOME 32
#define DISPOSITIVO_TAM_CONFIG 32

struct dispositivo
{
    uint8_t tipo;
    uint8_t nome[DISPOSITIVO_TAM_NOME];
    tam_t tamanho_bloco;
    tam_t (* leia_bloco)(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade);
    tam_t (* grava_bloco)(posicao_t dispositivo, byte_t * origem, posicao_t posicao, tam_t quantidade);
    tam_t tamanho;
    posicao_t config[DISPOSITIVO_TAM_CONFIG];
};

typedef struct dispositivo dispositivo_t;

#define DISPOSITIVO_TIPO_INDEFINIDO 0
#define DISPOSITIVO_TIPO_DISCO 1
#define DISPOSITIVO_TIPO_SERIAL 2
#define DISPOSITIVO_TIPO_TECLADO 3
#define DISPOSITIVO_TIPO_INTERFACE 4

#define DISPOSITIVO_NAO_ENCONTRADO 0


void dispositivo_inicia();
posicao_t dispositivo_registra(txt_t nome, uint8_t tipo, tam_t tamanho_bloco);
posicao_t dispositivo_busca(txt_t nome);
status_t dispositivo_status(posicao_t dispositivo);
status_t dispositivo_altera_leia(posicao_t dispositivo, tam_t (* leia_bloco)(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade));
status_t dispositivo_altera_grava(posicao_t dispositivo, tam_t (* grava_bloco)(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade));
status_t dispositivo_altera_tamanho(posicao_t dispositivo, tam_t tamanho);
status_t dispositivo_altera_config(posicao_t dispositivo, posicao_t posicao, posicao_t valor);
status_t dispositivo_leia_config(posicao_t dispositivo, posicao_t posicao, posicao_t * destino);
status_t dispositivo_tamanho(posicao_t dispositivo, tam_t * destino);
tam_t dispositivo_leia_bloco(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade);
tam_t dispositivo_grava_bloco(posicao_t dispositivo, byte_t * origem, posicao_t posicao, tam_t quantidade);
status_t dispositivo_remove(posicao_t dispositivo);
tam_t dispositivo_quantidade();


status_t dispositivo_disco_inicia();
status_t dispositivo_es_inicia();
#endif
