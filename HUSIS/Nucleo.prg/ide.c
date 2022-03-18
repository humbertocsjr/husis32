#include "husis.h"
#include "ide.h"
#include "dispositivo.h"

void ide_inicia()
{
    posicao_t ide0 = dispositivo_registra("ide1", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide1 = dispositivo_registra("ide2", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide2 = dispositivo_registra("ide3", DISPOSITIVO_TIPO_DISCO, 512);
    posicao_t ide3 = dispositivo_registra("ide4", DISPOSITIVO_TIPO_DISCO, 512);
}
