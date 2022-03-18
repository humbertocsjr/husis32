/*
 * Modulo de Entrada e Saida para PC com processador 386 ou superior
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.16 - Lido os argumentos do Multiboot
 */

#include "husis.h"
#include "es.h" 
#include "es_video.h"
#include "mem.h"
#include "processo.h"
#include "es_multiboot.h"

// Ponteiros do SmallerC Linker que apontam para o inicio e fim do binario
asm("extrn __start_allcode__");
asm("extrn __stop_alldata__");

// Estrutura Multiboot, informa ao Boot Loader o formato do nucleo e onde fica o ponto inicial
asm("section .text align 4");
asm("multiboot:");
asm("   dd 0x1badb002");
asm("   dd 65539");
asm("   dd -(0x1badb002 + (65539))");
asm("   dd multiboot");
asm("   dd __start_allcode__");
asm("   dd __stop_alldata__");
asm("   dd 0");
asm("   dd __start");

// Reserva 16 KiB para a pilha
asm("section .bss");
asm("   rb 16384");
asm("   pilha_topo:");


// ES_H


es_multiboot_dados_t * _es_multiboot;
uint32_t _es_multiboot_assinatura;

// Ponto de partida do executavel Multiboot
void _start()
{
    asm("cmp eax, 0x2badb002");
    asm("jne __cancela");
    asm("mov esp, pilha_topo");
    asm("push eax");
    asm("push ebx");
    asm("call _es_inicial");
    asm("__cancela:");
}

void es_inicial(uint32_t dados, uint32_t assinatura)
{
    // Define a pilha para o espaço reservado
    _es_multiboot = (es_multiboot_dados_t *) dados;
    _es_multiboot_assinatura = assinatura;
    // Executa o código do nucleo
    husis((txt_t)_es_multiboot->argumentos);
    // Loop infinito que desliga o processador nos intervalos
    while(-1)
    {
        asm("hlt");
    }
}

void es_escreva_8(uint16_t porta, uint8_t valor)
{
    // Os parametros iniciam no EBP + 8 e vão crescendo de 4 em 4 bytes
    uint8_t tmp = valor;                // Força o compilador a definir o EBP
    asm("mov dx, [ebp + 8]");           // Porta
    asm("mov al, [ebp + 12]");          // Valor
    asm("out dx, al");                  // Emite um valor na porta
}

uint8_t es_leia_8(uint16_t porta)
{
    // Os parametros iniciam no EBP + 8 e vão crescendo de 4 em 4 bytes
    // Enquanto as variaveis iniciam em EBP -4 e vão diminuindo de 4 em 4 bytes
    uint8_t tmp = 0;                    // Inicia a variavel EBP -4
    asm("mov dx, [ebp + 8]");           // Le a variavel porta
    asm("xor eax, eax");                // Limpa o registrador destino
    asm("in al, dx");                   // Le um valor da porta
    asm("mov [ebp - 4], al");           // Grava na variavel tmp
    return tmp;
}

void es_escreva_16(uint16_t porta, uint16_t valor)
{
    // Os parametros iniciam no EBP + 8 e vão crescendo de 4 em 4 bytes
    uint8_t tmp = valor;                // Força o compilador a definir o EBP
    asm("mov dx, [ebp + 8]");           // Porta
    asm("mov ax, [ebp + 12]");          // Valor
    asm("out dx, ax");                  // Emite um valor na porta
}

uint16_t es_leia_16(uint16_t porta)
{
    // Os parametros iniciam no EBP + 8 e vão crescendo de 4 em 4 bytes
    // Enquanto as variaveis iniciam em EBP -4 e vão diminuindo de 4 em 4 bytes
    uint8_t tmp = 0;                    // Inicia a variavel EBP -4
    asm("mov dx, [ebp + 8]");           // Le a variavel porta
    asm("xor eax, eax");                // Limpa o registrador destino
    asm("in ax, dx");                   // Le um valor da porta
    asm("mov [ebp - 4], ax");           // Grava na variavel tmp
    return tmp;
}


posicao_t es_nucleo_primeira_pagina()
{
    uint32_t tmp = 0;
    asm("mov dword [ebp -4], __start_allcode__");
    tmp --;
    tmp /= 4096;
    return tmp;
}

posicao_t es_nucleo_ultima_pagina()
{
    uint32_t tmp = 0;
    asm("mov dword [ebp -4], __stop_alldata__");
    tmp++;
    tmp /= 4096;
    return tmp;
}

void es_aplicar_areas_restritas(tam_t capacidade_paginas)
{
    for(posicao_t i = 256; i < capacidade_paginas; i++)
    {
        if(i < 3840 & i <= (_es_multiboot->memoria_alta / 4))
        {
            mem_grava(i, PROCESSO_VAZIO, 0);
        }
        else if(i >= 4096 & i <= (_es_multiboot->memoria_alta / 4))
        {
            mem_grava(i, PROCESSO_VAZIO, 0);
        }
    }
}

tam_t es_mem_tamanho()
{
    return _es_multiboot->memoria_alta * 1024;
}


// ES_VIDEO_H

uint16_t * _es_video_mem;               // Ponteiro para o inicio da memoria de video
uint16_t _es_video_largura;             // Largura da tela
uint16_t _es_video_altura;              // Altura da tela

void es_video_inicia()
{
    _es_video_mem = (uint16_t *)0xB8000;// Ponteiro da memória de video
    _es_video_altura = 25;              // Altura da tela
    _es_video_largura = 80;             // Largura da tela
}

void es_video_limpa(cor_t cor_texto, cor_t cor_fundo)
{
    for(tam_t y = 0; y < 25; y++)
    {
        for(tam_t x = 0; x < 80; x++)
        {
            // Preenche a tela com espaços
            _es_video_mem[y * _es_video_largura + x] = (uint16_t)' ' | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        }
    }
}

void es_video_escreva_c(tam_t x, tam_t y, uint8_t c, cor_t cor_texto, cor_t cor_fundo)
{
    if(x >= _es_video_largura | y >= _es_video_altura) 
        return;                         // Cancela caso tente escrever fora da tela
    _es_video_mem[y * _es_video_largura + x] = (uint16_t)c | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
}

void es_video_escreva_c_repetido(tam_t x, tam_t y, tam_t qtd, uint8_t c, cor_t cor_texto, cor_t cor_fundo, uint8_t limite)
{
    tam_t xmax = 0;
    if(limite == ES_VIDEO_LIMITE_LINHA)
    {
        // Limita a escrita a linha atual, interrompendo quando tentar escrever alem
        xmax = (y + 1) * _es_video_largura;
    }
    else if(limite == ES_VIDEO_LIMITE_TELA)
    {
        // Limia a escrita ao final da tela, interrompendo quando tentar escrever alem
        xmax = _es_video_altura * _es_video_largura;
    }
    tam_t j = 0;
    for(tam_t i = y * _es_video_largura + x; i < xmax; i++)
    {
        if(j >= qtd) break;             // Interrompe quando ultrapassa o limite
        _es_video_mem[i] = (uint16_t)c | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        j++;
    }
}

tam_t es_video_escreva_txt(tam_t x, tam_t y, tam_t tam, txt_t txt, cor_t cor_texto, cor_t cor_fundo, uint8_t limite)
{
    tam_t xmax = 0;
    if(limite == ES_VIDEO_LIMITE_LINHA)
    {
        // Limita a escrita a linha atual, interrompendo quando tentar escrever alem
        xmax = (y + 1) * _es_video_largura;
    }
    else if(limite == ES_VIDEO_LIMITE_TELA)
    {
        // Limia a escrita ao final da tela, interrompendo quando tentar escrever alem
        xmax =_es_video_altura * _es_video_largura;
    }
    tam_t j = 0;
    tam_t c = 0;
    for(tam_t i = y * _es_video_largura + x; i < xmax & c < tam; i++)
    {
        if(txt[j] == 0)
        {
            _es_video_mem[i] = (uint16_t)' ' | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        }
        else
        {
            _es_video_mem[i] = (uint16_t)txt[j++] | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        }
        c++;
    }
    return j;
}

tam_t es_video_escreva_nro(tam_t x, tam_t y, tam_t tam, uint32_t nro, cor_t cor_texto, cor_t cor_fundo)
{
    uint32_t tmp = nro;
    uint8_t digitos[20];
    digitos[0] = 0;
    posicao_t qtd = 0;
    for(posicao_t i = 0; i < 20 & tmp > 0; i++)
    {
        digitos[qtd] = (uint8_t)(tmp % 10);
        tmp = tmp / 10;
        qtd++;
    }
    if(qtd == 0) qtd++;
    for(posicao_t i = 0; i < tam; i++)
    {
        if(i < qtd)
        {
            es_video_escreva_c(x + i, y, (uint8_t)'0' + digitos[qtd - i - 1], cor_texto, cor_fundo);
        }
        else
        {
            es_video_escreva_c(x + i, y, ' ', cor_texto, cor_fundo);
        }
    }
    return qtd;
}

tam_t es_video_largura()
{
    return _es_video_largura;
}

tam_t es_video_altura()
{
    return _es_video_altura;
}

void es_video_oculta_cursor()
{
    es_escreva_8(0x3d4, 0xa);
    es_escreva_8(0x3d5, 0x20);
}

void es_video_exibe_cursor()
{
    uint8_t tmp;
    es_escreva_8(0x3d4, 0xa);
    tmp = es_leia_8(0x3d5) & 0xc0;
    es_escreva_8(0x3d5, tmp | 14);

    es_escreva_8(0x3D4, 0xb);
    tmp = es_leia_8(0x3d5) & 0xc0;
    es_escreva_8(0x3D5, tmp | 15);
}

void es_video_atualiza_cursor(tam_t x, tam_t y)
{
    uint16_t pos = y * _es_video_largura + x;

    es_escreva_8(0x3d4, 0xf);
    es_escreva_8(0x3d5, (uint8_t)(pos & 0xff));
    
    es_escreva_8(0x3d4, 0xe);
    es_escreva_8(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}
