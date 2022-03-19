#include "husis.h"
#include "dispositivo.h"

// DISPOSITIVOS DE DISCO

status_t dispositivo_disco_inicia()
{
    status_t ret = OK;
#ifndef SEM_DISQUETE
    DISPOSITIVO_INICIA(disquete, "Controlador de Disquete");
#endif
#ifndef SEM_IDE
    DISPOSITIVO_INICIA(ide, "Controlador IDE");
#endif
    return ret;
}
