/*
 * Modulo de Entrada e Saida para PC com processador 386 ou superior
 * Copyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)
 * 
 * Historia:
 * - 22.03.14 - Versão inicial
 * - 22.03.16 - Lido os argumentos do Multiboot
 * - 22.03.23 - Implementado Multitarefa, GDT e IDT
 */

#include "husis.h"
#include "es.h" 
#include "es_video.h"
#include "mem.h"
#include "processo.h"
#include "es_multiboot.h"
#include "es_int.h"

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

#pragma pack(push, 1)

struct es_gdt_item
{
    uint8_t limite0;
    uint8_t limite1;
    uint8_t base0;
    uint8_t base1;
    uint8_t base2;
    uint8_t acesso;
    uint8_t limite2;
    uint8_t base3;
};

typedef struct es_gdt_item es_gdt_item_t;

struct es_gdt_ponteiro
{
    uint16_t limite;
    uint32_t base;
};
typedef struct es_gdt_ponteiro es_gdt_ponteiro_t;

#define ES_GDT_TOTAL 5

struct es_idt_item
{
    uint16_t base0;
    uint16_t seletor;
    uint8_t zero;
    uint8_t opcoes;
    uint16_t base1;
};
typedef struct es_idt_item es_idt_item_t;

struct es_idt_ponteiro
{
    uint16_t limite;
    uint32_t base;
};
typedef struct es_idt_ponteiro es_idt_ponteiro_t;

#define ES_IDT_TOTAL 256

#define ES_IDT_PADRAO(int) extern void _es_int##int(); es_idt_grava(int, &_es_int##int, 0x8, 0x8e)

struct es_registradores
{
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t inutilizado;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t interrupcao;
    uint32_t codigo_erro;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

typedef struct es_registradores es_registradores_t;

#pragma pack(pop)

es_int_registro_t _es_int_manipuladores[ES_IDT_TOTAL];
es_gdt_item_t _es_gdt[ES_GDT_TOTAL];
es_gdt_ponteiro_t _es_gdt_ponteiro;
es_idt_item_t _es_idt[ES_IDT_TOTAL];
es_idt_ponteiro_t _es_idt_ponteiro;
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

void es_gdt_grava(posicao_t posicao, posicao_t base, posicao_t limite, uint8_t acesso, uint8_t opcoes)
{
    _es_gdt[posicao].base0   = (uint8_t)base & 0xff;
    _es_gdt[posicao].base1   = (uint8_t)(base >> 8) & 0xff;
    _es_gdt[posicao].base2   = (uint8_t)(base >> 16) & 0xff;
    _es_gdt[posicao].base3   = (uint8_t)(base >> 24) & 0xff;
    _es_gdt[posicao].limite0 = (uint8_t)limite & 0xff;
    _es_gdt[posicao].limite1 = (uint8_t)(limite >> 8) & 0xff;
    _es_gdt[posicao].limite2 = (uint8_t)((limite >> 16) & 0x0f) | ((opcoes << 4) & 0xf0 );
    _es_gdt[posicao].acesso  = acesso;
}

void es_idt_grava(posicao_t posicao, posicao_t base, uint16_t seletor, uint8_t opcoes)
{
    _es_idt[posicao].base0   = base & 0xffff;
    _es_idt[posicao].base1   = (base >> 16) & 0xffff;
    _es_idt[posicao].seletor = seletor;
    _es_idt[posicao].zero    = 0;
    _es_idt[posicao].opcoes  = opcoes /*| 0x60*/;
}

void es_int_inicia()
{
    
    for (posicao_t i = 0; i < ES_GDT_TOTAL; i ++)
    {
        es_gdt_grava(i, 0,0,0,0);
    }
    
    es_gdt_grava(1, 0, 0xFFFFFFFF, 0x9a, 0xc);
    es_gdt_grava(2, 0, 0xFFFFFFFF, 0x92, 0xc);
    es_gdt_grava(3, 0, 0xFFFFFFFF, 0xfa, 0xc);
    es_gdt_grava(4, 0, 0xFFFFFFFF, 0xf2, 0xc);
    
    _es_gdt_ponteiro.base = (uint32_t)&_es_gdt;
    _es_gdt_ponteiro.limite = (ES_GDT_TOTAL * sizeof(es_gdt_item_t)) - 1;
    
    asm("lgdt [__es_gdt_ponteiro]");
    asm("mov ax, 0x10");
    asm("mov ds, ax");
    asm("mov es, ax");
    asm("mov fs, ax");
    asm("mov gs, ax");
    asm("mov ss, ax");
    asm("jmp 0x8:es_gdt_continua");
    asm("es_gdt_continua:");
    
    extern void _es_intextra();
    for (posicao_t i = 0; i < ES_IDT_TOTAL; i ++)
    {
         es_idt_grava(i, &_es_intextra, 0x8, 0x8e);
         _es_int_manipuladores[i].interrupcao = i;
         _es_int_manipuladores[i].manipulador = 0;
    }
    
    ES_IDT_PADRAO(0);
    ES_IDT_PADRAO(1);
    ES_IDT_PADRAO(2);
    ES_IDT_PADRAO(3);
    ES_IDT_PADRAO(4);
    ES_IDT_PADRAO(5);
    ES_IDT_PADRAO(6);
    ES_IDT_PADRAO(7);
    ES_IDT_PADRAO(8);
    ES_IDT_PADRAO(9);
    ES_IDT_PADRAO(10);
    ES_IDT_PADRAO(11);
    ES_IDT_PADRAO(12);
    ES_IDT_PADRAO(13);
    ES_IDT_PADRAO(14);
    ES_IDT_PADRAO(15);
    ES_IDT_PADRAO(16);
    ES_IDT_PADRAO(17);
    ES_IDT_PADRAO(18);
    ES_IDT_PADRAO(19);
    ES_IDT_PADRAO(20);
    ES_IDT_PADRAO(21);
    ES_IDT_PADRAO(22);
    ES_IDT_PADRAO(23);
    ES_IDT_PADRAO(24);
    ES_IDT_PADRAO(25);
    ES_IDT_PADRAO(26);
    ES_IDT_PADRAO(27);
    ES_IDT_PADRAO(28);
    ES_IDT_PADRAO(29);
    ES_IDT_PADRAO(30);
    ES_IDT_PADRAO(31);
    ES_IDT_PADRAO(32);
    ES_IDT_PADRAO(33);
    ES_IDT_PADRAO(34);
    ES_IDT_PADRAO(35);
    ES_IDT_PADRAO(36);
    ES_IDT_PADRAO(37);
    ES_IDT_PADRAO(38);
    ES_IDT_PADRAO(39);
    ES_IDT_PADRAO(40);
    ES_IDT_PADRAO(41);
    ES_IDT_PADRAO(42);
    ES_IDT_PADRAO(43);
    ES_IDT_PADRAO(44);
    ES_IDT_PADRAO(45);
    ES_IDT_PADRAO(46);
    ES_IDT_PADRAO(47);
    ES_IDT_PADRAO(48);
    ES_IDT_PADRAO(49);
    
    _es_idt_ponteiro.base = (uint32_t)&_es_idt;
    _es_idt_ponteiro.limite = (ES_IDT_TOTAL * sizeof(es_idt_item_t)) - 1;
    
    asm("lidt [__es_idt_ponteiro]");
    
    // Remapeia o as IRQs do PIT de 0-15 para 32-47 para evitar cruzamentos com as interrupcoes do processador, o que geraria erros.
    es_escreva_8(0x20, 0x11);
    es_escreva_8(0xA0, 0x11);
    es_escreva_8(0x21, 0x20);
    es_escreva_8(0xA1, 0x28);
    es_escreva_8(0x21, 0x04);
    es_escreva_8(0xA1, 0x02);
    es_escreva_8(0x21, 0x01);
    es_escreva_8(0xA1, 0x01);
    es_escreva_8(0x21, 0x0);
    es_escreva_8(0xA1, 0x0);
    
    // Define o timer do computador
    es_escreva_8(0x43, 0x36);
    posicao_t timer = 1193180 / 50; // 50 hz
    es_escreva_8(0x40, timer & 0xff);
    es_escreva_8(0x40, (timer >> 8) & 0xff);
    
    asm("sti");
}

posicao_t _es_contador = 0;

posicao_t _es_multitarefa_ss = 0;
posicao_t _es_multitarefa_esp = 0;

void es_processar_int(es_registradores_t regs)
{
    if((posicao_t)_es_int_manipuladores[regs.interrupcao].manipulador > 0)
    {
       _es_int_manipuladores[regs.interrupcao].manipulador(regs.eax, regs.ebx, regs.ecx, regs.edx, regs.interrupcao, regs.codigo_erro); 
    }
    if(regs.interrupcao == 32)
    {
        // Timer
        es_video_escreva_nro(0,0,10,regs.interrupcao, COR_PRETO, COR_BRANCO);
        es_video_escreva_nro(10,0,10,regs.codigo_erro, COR_PRETO, COR_BRANCO);
        es_video_escreva_nro(20,0,10,_es_contador, COR_PRETO, COR_BRANCO);
        extern processo_t _processo_atual;
        extern processo_dados_t _processos[PROCESSO_TOTAL];
        processo_t proximo = processo_proximo_processo();
        if(proximo != _processo_atual)
        {
            asm("cli");
            asm("xor eax, eax");
            asm("mov ax, ss");
            asm("mov [__es_multitarefa_ss], eax");
            asm("mov eax, esp");
            asm("mov [__es_multitarefa_esp], eax");
            _processos[_processo_atual].seletor = _es_multitarefa_ss;
            _processos[_processo_atual].pilha = _es_multitarefa_esp;
            
            _es_multitarefa_ss = _processos[proximo].seletor;
            _es_multitarefa_esp = _processos[proximo].pilha;
            _processo_atual = proximo;
            asm("mov eax, [__es_multitarefa_ss]");
            asm("mov ss, ax");
            asm("mov eax, [__es_multitarefa_esp]");
            asm("mov esp, eax");
            asm("sti");
        }
        _es_contador++;
    }
    if(regs.interrupcao >= 40)
    {
        es_escreva_8(0xa0, 0x20);
    }
    es_escreva_8(0x20, 0x20);
}

status_t es_int_altera(posicao_t interrupcao, void (* manipulador)(posicao_t reg1, posicao_t reg2, posicao_t reg3, posicao_t reg4, posicao_t interrupcao, posicao_t codigo_erro))
{
    if(interrupcao >= ES_IDT_TOTAL)
        return ERRO_ESTOURO_DE_CAPACIDADE;
    _es_int_manipuladores[interrupcao].manipulador = manipulador;
    return OK;
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
    // Monta o primeiro MB (Posição 32KiB ate 640KiB)
    for(posicao_t i = 8; i < 160; i++)
    {
        mem_grava(i, PROCESSO_VAZIO, 0);
    }
    // Monta o restante da memoria (Pulando entre 15MiB e 16MiB)
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
