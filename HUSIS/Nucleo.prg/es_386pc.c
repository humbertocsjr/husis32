#include "es.h" 

asm("section .text align 4");
asm("multiboot:");
asm("dd 0x1badb002");
asm("dd 65539");
asm("dd -(0x1badb002 + (65539))");
asm("dd multiboot");
asm("extrn __start_allcode__");
asm("dd __start_allcode__");
asm("extrn __stop_alldata__");
asm("dd __stop_alldata__");
asm("dd 0");
asm("dd __start");
asm("section .bss");
asm("rb 16384");
asm("pilha_topo:");

void _start()
{
    asm("mov esp, pilha_topo");
    husis("");
    while(-1)
    {
        asm("hlt");
    }
}

void es_escreva_8(uint16_t porta, uint8_t valor)
{
    uint8_t tmp = valor;
    asm("mov dx, [ebp + 8]");
    asm("mov al, [ebp + 12]");
    asm("out dx, al");
}

uint8_t es_leia_8(uint16_t porta)
{
    uint8_t tmp = 0;
    asm("mov dx, [ebp + 8]");
    asm("xor eax, eax");
    asm("in al, dx");
    asm("mov [ebp - 4], al");
    return tmp;
}

void es_escreva_16(uint16_t porta, uint16_t valor)
{
    uint8_t tmp = valor;
    asm("mov dx, [ebp + 8]");
    asm("mov ax, [ebp + 12]");
    asm("out dx, ax");
}

uint16_t es_leia_16(uint16_t porta)
{
    uint8_t tmp = 0;
    asm("mov dx, [ebp + 8]");
    asm("xor eax, eax");
    asm("in ax, dx");
    asm("mov [ebp - 4], ax");
    return tmp;
}

uint16_t * _es_video_mem;
uint16_t _es_video_largura;
uint16_t _es_video_altura;

void es_video_iniciar()
{
    _es_video_mem = (uint16_t *)0xB8000;
    _es_video_altura = 25;
    _es_video_largura = 80;
}

void es_video_limpar(cor_t cor_texto, cor_t cor_fundo)
{
    for(tam_t y = 0; y < 80; y++)
    {
        for(tam_t x = 0; x < 80; x++)
        {
            _es_video_mem[y * _es_video_largura + x] = (uint16_t)' ' | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        }
    }
}

void es_video_escreva_c(tam_t x, tam_t y, uint8_t c, cor_t cor_texto, cor_t cor_fundo)
{
    if(x >= _es_video_largura | y >= _es_video_altura) return;
    _es_video_mem[y * _es_video_largura + x] = (uint16_t)c | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
}

void es_video_escreva_c_repetido(tam_t x, tam_t y, tam_t qtd, uint8_t c, cor_t cor_texto, cor_t cor_fundo, uint8_t limite)
{
    tam_t xmax = 0;
    if(limite == ES_VIDEO_LIMITE_LINHA)
    {
        xmax = (y + 1) * _es_video_largura;
    }
    else if(limite == ES_VIDEO_LIMITE_TELA)
    {
        xmax = _es_video_altura * _es_video_largura;
    }
    tam_t j = 0;
    for(tam_t i = y * _es_video_largura + x; i < xmax; i++)
    {
        if(j >= qtd) break;
        _es_video_mem[i] = (uint16_t)c | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
        j++;
    }
}

void es_video_escreva_txt(tam_t x, tam_t y, tam_t tam, txt_t txt, cor_t cor_texto, cor_t cor_fundo, uint8_t limite)
{
    tam_t xmax = 0;
    if(limite == ES_VIDEO_LIMITE_LINHA)
    {
        xmax = (y + 1) * _es_video_largura;
    }
    else if(limite == ES_VIDEO_LIMITE_TELA)
    {
        xmax =_es_video_altura * _es_video_largura;
    }
    tam_t j = 0;
    for(tam_t i = y * _es_video_largura + x; i < xmax; i++)
    {
        if(txt[j] == 0) break;
        _es_video_mem[i] = (uint16_t)txt[j++] | (uint16_t)((cor_texto | cor_fundo << 4) << 8);
    }
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
    es_escreva_8(0x3d4, 0xb);
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
