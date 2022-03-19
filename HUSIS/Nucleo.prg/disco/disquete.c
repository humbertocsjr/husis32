
#include "../husis.h"
#include "disquete.h"
#include "../dispositivo.h"

status_t disquete_registra(txt_t nome, posicao_t id, tam_t cilindros, tam_t cabecas, tam_t setores)
{
    status_t ret = OK;
    posicao_t disco = dispositivo_registra(nome, DISPOSITIVO_TIPO_DISCO, 512);
    VALIDA(dispositivo_status(disco));
    VALIDA(dispositivo_altera_config(disco, 1, cilindros));
    VALIDA(dispositivo_altera_config(disco, 2, cabecas));
    VALIDA(dispositivo_altera_config(disco, 3, setores));
    VALIDA(dispositivo_altera_tamanho(disco, cilindros*cabecas*setores*512));
    return ret;
}

status_t disquete_inicia()
{
    status_t ret = OK;
    VALIDA(disquete_registra("disquete0_720", 0, 40, 2, 18));
    VALIDA(disquete_registra("disquete0_1440", 0, 80, 2, 18));
    VALIDA(disquete_registra("disquete0_360", 0, 40, 2, 9));
    VALIDA(disquete_registra("disquete0_1200", 0, 80, 2, 15));
    VALIDA(disquete_registra("disquete1_720", 1, 40, 2, 18));
    VALIDA(disquete_registra("disquete1_1440", 1, 80, 2, 18));
    VALIDA(disquete_registra("disquete1_360", 1, 40, 2, 9));
    VALIDA(disquete_registra("disquete1_1200", 1, 80, 2, 15));
    return ret;
}

