/*
 * HUSIS - Arquivo base do nucleo
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.16 - Comando de alteracao do percentual da tela de progresso
 */

#ifndef HUSIS_H
#define HUSIS_H

/*
 * Exemplo de uso do status_t:
 * 
 * status_t funcao()
 * {
 *     status_t ret = OK;
 *     VALIDA(comando_que_retorna_status());
 *     return ret;
 * }
 */

#define VALIDA(comando) if((ret = comando) != OK) return ret

#define ENTRE(valor, min, max) (valor >= min & valor <= max)

typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned int size_t;

typedef unsigned char byte_t;
typedef unsigned int processo_t;
typedef unsigned int tam_t;
typedef unsigned int posicao_t;
typedef char * txt_t;
typedef unsigned char cor_t;
typedef unsigned short status_t;

#define OK 0
#define IGUAL 0
#define ERRO_ACESSO_NEGADO 1
#define ERRO_NAO_ENCONTRADO 2
#define DIFERENTE 3
#define ERRO_DIFERENTE 3
#define ERRO_ARGUMENTO_INVALIDO 4
#define ERRO_DADOS_CORROMPIDOS 5
#define ERRO_ESTOURO_DE_CAPACIDADE 10

#define COR_PRETO 0
#define COR_AZUL 1
#define COR_VERDE 2
#define COR_CIANO 3
#define COR_VERMELHO 4
#define COR_MAGENTA 5
#define COR_MARROM 6
#define COR_CINZA_CLARO 7
#define COR_CINZA_ESCURO 8
#define COR_AZUL_CLARO 9
#define COR_VERDE_CLARO 10
#define COR_CIANO_CLARO 11
#define COR_VERMELHO_CLARO 12
#define COR_MAGENTA_CLARO 13
#define COR_MARROM_CLARO 14
#define COR_BRANCO 15


void husis_progresso(txt_t texto1, txt_t texto2);
void husis_progresso_erro(txt_t erro, status_t codigo);
void husis_progresso_altera(int8_t percentual);

#include "config.h"

#endif
