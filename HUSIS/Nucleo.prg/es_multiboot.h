/*
 * Cabecalho Multiboot
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.16 - Versão inicial
 */

#ifndef ES_MULTIBOOT_H
#define ES_MULTIBOOT_H

struct es_multiboot_memoria
{
    uint32_t tamanho;
    uint32_t endereco_baixo;
    uint32_t endereco_alto;
    uint32_t tamanho_baixo;
    uint32_t tamanho_alto;
    uint32_t tipo;
};

typedef struct es_multiboot_memoria es_multiboot_memoria_t;

struct es_multiboot_dados
{
    uint32_t opcoes;
    uint32_t memoria_baixa;
    uint32_t memoria_alta;
    uint32_t dispositivo_inicial;
    uint32_t argumentos;
    uint32_t modulos_qtd;
    uint32_t modulos_lista;
    uint32_t aout_tabela_tam;
    uint32_t aout_texto_tam;
    uint32_t aout_endereco;
    uint32_t aout_reservado;
    uint32_t elf_numero;
    uint32_t elf_tam;
    uint32_t elf_endereco;
    uint32_t elf_outro;
    uint32_t memoria_mapa_tam;
    uint32_t memoria_mapa;
};

typedef struct es_multiboot_dados es_multiboot_dados_t;


#endif
