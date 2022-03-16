# Projeto HUSIS

Sistema Operacional para PCs com processadores 386 ou superiores, este projeto está em fase inicial de protótipo.

## Estrutura

Este projeto está sendo feito de uma forma que seja facilmente portável para outros processadores, então todo o código do núcleo que é dependente de plataforma fica concentrado no arquivo es_(arquitetura).c, sendo o restante do código independente de plataforma.

O núcleo é dividido em módulos:

- iut.c - Interface com o Usuario via Texto
    - Responsável por gerar as telas e componentes na tela que o usuário interage
- es_(arquitetura).c - Modulo de Entrada e Saida Nativo
    - Armazena todo o código dependente de arquitetura do núcleo
    - Onde fica o ponto inicial de execução do sistema operacional (Rotina _es_inicial)
    - Responsável pela comunicação com o processador
- sisarq.c - Sistema de Arquivos Virtual
    - Fornece a API necessária para implementar os sistemas de arquivos
- nucleo.c - Arquivo principal do núcleo, onde liga os demais módulos

## Ordem de execução ao ligar a maquina

- es_(arquitetura).c - O Gerenciador de Inicialização (Boot Loader) carrega o Núcleo para a memória e inicia a execução pela rotina _start ou _es_inicial
- nucleo.c - O Núcleo inicia o sistema de video para que possa mostrar mensagens para o usuário, na rotina Husis.
- iut.c - Limpa a tela e é utilizado pelo nucleo para exibir as mensagens

## Licenciamento

Todo código fonte deste projeto é distribuido através da licença BSD, conforme arquivo license.txt.

Existem outros projetos incluidos em sua forma de código fonte ou binária e estes são distribuídos conforme suas respectivas licenças:
- Existem binários do GRUB Legacy no diretório boot, apenas é distribuído para facilitar como uma alternativa possível para boot, não sendo sua única forma
- Existem projetos dos compiladores que são usados por este projeto, estão disponibilizados no diretório Desenvolvimento

## Recursos implementados

- Novo nucleo com inicialização Multiboot
- Separação do código dependente de plataforma no módulo "es" (Entrada e Saída), podendo ser substituído por outros módulos conforme a plataforma destino
- Protótipo da Interface com o Usuário

## Recursos previstos

- Suporte ao sistema de arquivos Minix v1
- Biblioteca C para o SmallerC
- Implementar carregamento de executáveis a.out
- Porte do FASM e do SmallerC para o HUSIS
- Criar um sistema inicial para carregar o nucleo, e não precisar do SYSLINUX/LILI/GRUB
