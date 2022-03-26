#include "../husis.h"
#include "../dispositivo.h"




status_t ide_inicia()
{
    status_t ret = OK;
    posicao_t ide0 = dispositivo_registra("IDE0", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide1 = dispositivo_registra("IDE1", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide2 = dispositivo_registra("IDE2", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide3 = dispositivo_registra("IDE3", DISPOSITIVO_TIPO_DISCO, 512);
    return ret;
}
