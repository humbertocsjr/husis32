# Projeto HUSIS

Sistema Operacional para IBM PC e compatíveis



Estou criando um sistema operacional completamente feito em Assembly compatível com o 8086.



Ainda está em fase de prototipação, tendo apenas o bootloader completo.



Acompanhe essa aventura via o [BLOG](http://humbertocsjr.dev.br)



## Recursos implementados

- Sistema Inicial (Bootloader)

- Protótipo do Núcleo

- Suporte ao Sistema de Arquivos MinixFS v1

- Definido formato do executável



## Recursos previstos

- Interpretador BASIC compatível com o MikeOS BASIC

- Compilador BASIC

- Porte do Old-School Assembly

- Interface gráfica com o usuário



## Como compilar

#### Pré requisitos

- DOSBOX-X com LFN ativado

- Linux para gerar as imagens (Em breve será substituido pela ferramenta minixfs multiplataforma)

#### Compilando a parte do DOSBOX-X

- Inicie a sessão do DOSBOX montando como unidade C: a pasta raiz desse projeto

- Execute o comando abaixo:

```batch
compilar.bat
```

Ele deve automaticamente compilar cada sub projeto

#### Compilando a parte do Linux

- Para criar as imagens (.img) se deve executar o comando:

```shell
make
```

Ele vai pegar os binarios criador pela parte do DOSBOX-X e gerar as imagens de 360 KiB e 1440 KiB
