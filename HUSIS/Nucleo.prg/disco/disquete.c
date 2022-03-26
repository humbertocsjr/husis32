
#include "../husis.h"
#include "../dispositivo.h"
#include "../es_int.h"
#include "../processo.h"

// Registrador de Saida Digital
#define DISQUETE_PORTA_RSD 0x3f2
#define DISQUETE_RSD_CONTROLADOR0 0
#define DISQUETE_RSD_CONTROLADOR1 1
#define DISQUETE_RSD_CONTROLADOR2 2
#define DISQUETE_RSD_CONTROLADOR3 3
#define DISQUETE_RSD_REDEFINE 4
#define DISQUETE_RSD_DMA 8
#define DISQUETE_RSD_MOTOR0 16
#define DISQUETE_RSD_MOTOR1 32
#define DISQUETE_RSD_MOTOR2 64
#define DISQUETE_RSD_MOTOR3 128

// Registrador de Status Principal
#define DISQUETE_PORTA_RSP 0x3f4
#define DISQUETE_RSP_CONTROLADOR0 1
#define DISQUETE_RSP_CONTROLADOR1 2
#define DISQUETE_RSP_CONTROLADOR2 4
#define DISQUETE_RSP_CONTROLADOR3 8
#define DISQUETE_RSP_OCUPADO 16
#define DISQUETE_RSP_DMA 32
#define DISQUETE_RSP_ES_DADOS 64
#define DISQUETE_RSP_REG_DADOS 128

// Registrador Primeiro Entrar Primeiro Sair
#define DISQUETE_PORTA_PEPS 0x3f5

// Registrador Controlador
#define DISQUETE_PORTA_CONTROL 0x3f7

#define DISQUETE_CMD_LEIA_TRILHA 2
#define DISQUETE_CMD_PARAMETROS 3
#define DISQUETE_CMD_LEIA_STATUS 4
#define DISQUETE_CMD_GRAVA_DADOS 5
#define DISQUETE_CMD_LEIA_DADOS 6
#define DISQUETE_CMD_RECALIBRA 7
#define DISQUETE_CMD_LEIA_ULTIMO_STATUS 8
#define DISQUETE_CMD_GRAVA_DADOS_EXCLUIDOS 9
#define DISQUETE_CMD_LEIA_ID 10
#define DISQUETE_CMD_LEIA_DADOS_EXCLUIDOS 12
#define DISQUETE_CMD_FORMATA_TRILHA 13
#define DISQUETE_CMD_LEIA_REG 14
#define DISQUETE_CMD_VA_PARA 15
#define DISQUETE_CMD_VERSAO 16
#define DISQUETE_CMD_LEIA_IGUAL 17
#define DISQUETE_CMD_MODO_PERPENDICULAR 18
#define DISQUETE_CMD_CONFIGURA 19
#define DISQUETE_CMD_BLOQUEIA 20
#define DISQUETE_CMD_VERIFICA 22
#define DISQUETE_CMD_LEIA_IGUAL_OU_MENOR 25
#define DISQUETE_CMD_LEIA_IGUAL_OU_MAIOR 29

volatile posicao_t _disquete_req_recebida = 0;
byte_t * _disquete_temp = 0;

void disquete_manipulador(posicao_t reg1, posicao_t reg2, posicao_t reg3, posicao_t reg4, posicao_t interrupcao, posicao_t codigo_erro)
{
    _disquete_req_recebida = 1;
}

void disquete_leia_dma()
{
    es_escreva_8(0xa, 0x6); // Mascara o canal 2 DMA
    es_escreva_8(0xb, 0x56); // Efetua uma transferencia de leitura via canal 2 DMA
    es_escreva_8(0xa, 0x2); // Desmascara o canal 2 DMA
}

void disquete_grava_dma()
{
    es_escreva_8(0xa, 0x6); // Mascara o canal 2 DMA
    es_escreva_8(0xb, 0x5a); // Efetua uma transferencia de gravacao via canal 2 DMA
    es_escreva_8(0xa, 0x2); // Desmascara o canal 2 DMA
}

status_t disquete_escreva_8(posicao_t id, uint8_t dado)
{
    for(posicao_t i = 0; i < 75; i++)
    {
        if((es_leia_8(DISQUETE_PORTA_RSP & 0xc0)) == 0x80)
        {
            es_escreva_8(DISQUETE_PORTA_PEPS, dado);
            return OK;
        }
        aguarda(1);
    }
    return ERRO_TEMPO_ESGOTADO;
}

status_t disquete_leia_8(posicao_t id, uint8_t * dado)
{
    
}

status_t disquete_ultimo_status(posicao_t id)
{
    
}

status_t disquete_redefine(posicao_t id)
{
    status_t ret = OK;
    // Para tudo
    es_escreva_8(DISQUETE_PORTA_RSD, DISQUETE_RSD_DMA);
    aguarda(10);
    // Reinicia tudo
    es_escreva_8(DISQUETE_PORTA_CONTROL, 0);
    _disquete_req_recebida = 0;
    es_escreva_8(DISQUETE_PORTA_RSD, DISQUETE_RSD_DMA | DISQUETE_RSD_REDEFINE);
    // Aguarda interrupcao retornar
    VALIDA(aguarda_valor(50, &_disquete_req_recebida));
    
    return ret;
}

tam_t disquete_leia_bloco(posicao_t dispositivo, byte_t * destino, posicao_t posicao, tam_t quantidade)
{
    return 0;
}


tam_t disquete_grava_bloco(posicao_t dispositivo, byte_t * origem, posicao_t posicao, tam_t quantidade)
{
    return 0;
}

status_t disquete_registra(txt_t nome, posicao_t id, tam_t cilindros, tam_t cabecas, tam_t setores)
{
    status_t ret = OK;
    posicao_t disco = dispositivo_registra(nome, DISPOSITIVO_TIPO_DISCO, 512);
    VALIDA(dispositivo_status(disco));
    VALIDA(dispositivo_altera_config(disco, 0, id));
    VALIDA(dispositivo_altera_config(disco, 1, cilindros));
    VALIDA(dispositivo_altera_config(disco, 2, cabecas));
    VALIDA(dispositivo_altera_config(disco, 3, setores));
    VALIDA(dispositivo_altera_tamanho(disco, cilindros * cabecas * setores * 512));
    VALIDA(dispositivo_altera_leia(disco, &disquete_leia_bloco));
    VALIDA(dispositivo_altera_grava(disco, &disquete_grava_bloco));
    return ret;
}

status_t disquete_inicia()
{
    status_t ret = OK;
    
    // Codigo anulado enquanto estiver em fase de prototipo
    return ret;
    
    posicao_t cmos_dados = 0;
    posicao_t disquete0 = 0;
    posicao_t disquete1 = 0;
    
    es_escreva_8(0x70, 0);
    cmos_dados = es_leia_8(0x71);
    if(cmos_dados == 0) return ret;
    disquete0 = cmos_dados & 0xf;
    disquete1 = (cmos_dados >> 4) & 0xf;
    
    VALIDA(mem_aloca_pag_baixa(PROCESSO_NUCLEO, 64/4, &_disquete_temp));
    VALIDA(es_int_altera(38, &disquete_manipulador));
    
    // Inicializa o Disquete 
    es_escreva_8(0xa0, 0x6); // Mascara o canal 2 DMA
    es_escreva_8(0xd8, 0xff); // Reinicia a chave principal
    es_escreva_8(0x4, (uint8_t)(((posicao_t)_disquete_temp >> 4) & 0xff)); // Grava o endereco temp
    es_escreva_8(0x4, (uint8_t)(((posicao_t)_disquete_temp >> 12) & 0xff));
    es_escreva_8(0xd8, 0xff); // Reinicia a chave principal
    es_escreva_8(0x5, 0xff); // 0x23ff Trilhas no disquete 3.5 1.44
    es_escreva_8(0x5, 0x23);
    es_escreva_8(0x80, 0x0); // Registro de pagina externa
    es_escreva_8(0xa0, 0x2); // Desmascara o canal 2 DMA
    
    // Registra os disquetes
    if(ENTRE(disquete0, 3, 5)) VALIDA(disquete_registra("Disquete0_720", 0, 40, 2, 18));
    if(ENTRE(disquete0, 4, 5)) VALIDA(disquete_registra("Disquete0_1440", 0, 80, 2, 18));
    if(ENTRE(disquete0, 1, 2)) VALIDA(disquete_registra("Disquete0_360", 0, 40, 2, 9));
    if(disquete0 == 2) VALIDA(disquete_registra("Disquete0_1200", 0, 80, 2, 15));
    if(ENTRE(disquete1, 3, 5)) VALIDA(disquete_registra("Disquete1_720", 1, 40, 2, 18));
    if(ENTRE(disquete1, 4, 5)) VALIDA(disquete_registra("Disquete1_1440", 1, 80, 2, 18));
    if(ENTRE(disquete1, 1, 2)) VALIDA(disquete_registra("Disquete1_360", 1, 40, 2, 9));
    if(disquete1 == 2) VALIDA(disquete_registra("Disquete1_1200", 1, 80, 2, 15));
    
    //if(disquete0 > 0) disquete_redefine(0);
    //if(disquete1 > 0) disquete_redefine(1);
    return ret;
}

