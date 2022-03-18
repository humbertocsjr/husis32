
#include "husis.h"
#include "disquete.h"
#include "dispositivo.h"
 
void disquete_inicia()
{
    posicao_t disquete0_720 = dispositivo_registra("disquete0_720", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete0_1440 = dispositivo_registra("disquete0_1440", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete0_360 = dispositivo_registra("disquete0_360", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete0_1200 = dispositivo_registra("disquete0_1200", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete1_720 = dispositivo_registra("disquete1_720", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete1_1440 = dispositivo_registra("disquete1_1440", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete1_360 = dispositivo_registra("disquete1_360", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t disquete1_1200 = dispositivo_registra("disquete1_1200", DISPOSITIVO_TIPO_DISCO, 512);
}

