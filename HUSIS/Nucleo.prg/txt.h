#ifndef TXT_H
#define TXT_H
#include "husis.h"
tam_t txt_tam(txt_t texto, tam_t capacidade);
status_t txt_copia(txt_t origem, txt_t destino, tam_t capacidade);
status_t txt_igual(txt_t txt1, txt_t txt2, tam_t capacidade);
#endif
