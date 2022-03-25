#ifndef ES_INT_H
#define ES_INT_H

struct es_int_registro
{
    posicao_t interrupcao;
    void (* manipulador)(posicao_t reg1, posicao_t reg2, posicao_t reg3, posicao_t reg4, posicao_t interrupcao, posicao_t codigo_erro);
};

typedef struct es_int_registro es_int_registro_t;

void es_int_inicia();
status_t es_int_altera(posicao_t interrupcao, void (* manipulador)(posicao_t reg1, posicao_t reg2, posicao_t reg3, posicao_t reg4, posicao_t interrupcao, posicao_t codigo_erro));

#endif
