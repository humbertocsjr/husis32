#include "husis.h"
#include "processo.h" 

processo_t _processo_atual = PROCESSO_NUCLEO;

processo_t processo_atual()
{
    return _processo_atual;
}
