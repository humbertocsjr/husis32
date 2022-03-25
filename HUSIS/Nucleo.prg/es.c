#include "husis.h"
#include "dispositivo.h"

// DISPOSITIVOS DE ENTRADA E SAIDA

status_t dispositivo_es_inicia()
{
    status_t ret = OK;
#ifndef SEM_TECLADO
    DISPOSITIVO_INICIA(teclado, "Controlador de Teclado");
#endif
#ifndef SEM_INTERFACE
    DISPOSITIVO_INICIA(interface, "Controlador de Interface com Usuario");
#endif
    return ret;
}
 
