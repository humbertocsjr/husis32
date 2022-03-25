#include "../husis.h"
#include "../dispositivo.h"
#include "../iut.h"

posicao_t _interface = 0;



tam_t interface_grava_bloco(posicao_t dispositivo, byte_t * origem, posicao_t posicao, tam_t quantidade)
{
    iut_pacote_t * pacotes = (iut_pacote_t *) (posicao_t) origem;
    for(posicao_t i = 0; i < quantidade; i ++)
    {
        switch(pacotes[i].acao)
        {
            case IUT_ACAO_NOVA_JANELA:
                iut_nova_janela(pacotes[i].controle, pacotes[i].x, pacotes[i].y, pacotes[i].largura, pacotes[i].altura, pacotes[i].texto, pacotes[i].opcoes);
                break;
            case IUT_ACAO_NOVO_ROTULO:
                iut_novo_rotulo(pacotes[i].controle, pacotes[i].x, pacotes[i].y, pacotes[i].largura, pacotes[i].texto);
                break;
        }
    }
    return quantidade;
}

status_t interface_inicia() 
{
    status_t ret = OK;
    _interface = dispositivo_registra("Interface", DISPOSITIVO_TIPO_INTERFACE, sizeof(iut_pacote_t));
    VALIDA(dispositivo_status(_interface));
    VALIDA(dispositivo_altera_grava(_interface, &interface_grava_bloco));
    
    
    return ret;
}
