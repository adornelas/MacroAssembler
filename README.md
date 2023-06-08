# MacroAssembler

Repo for CIC0104 - Software Básico project

Equipe:

- André Dornelas Sanches - 17/0099369
- Maria Cláudia - 17/0109968

Observação: os programas foram feitos para linux (Ubuntu), mas o montador foi feito em C++

## Como compilar

## Como usar o montador

Para montar o programa, é preciso digitar:

```
make clean
```

E depois

```
make
```

É importante notar que a versão de c++ utilizada é a c++20!

### Para tradução do código

```
./montador meuprograma saida.obj
```

O resultado do pré-processamento é apenas o arquivo com a retirada de espaços desnecessários e todo em maiúsculo.

A tradução ainda não está implementada, apenas o pré-processamento.


### Sobre a indentificação dos erros 

Os seguintes erros estão sendo identificados e classificados como: 

- Dois rotulos na mesma linha
- Rótulo não definido na seção de TEXT
- Dado não definido na seção DATA
- Seção TEXT faltante
- EXTERN e PUBLIC em arquivo que NAO tenha BEGIN e END
- Erros léxicos (caracteres especiais fora o “_” (underscore) ou “numero inicial nos rótulos)

Vale notar que o arquivo onde os erros estão sendo indicados é o arquivo pré-processado, com nome de ```preprocessedFile```.

Assumiu-se que o usuario nao vai usar numeros com pontos flutuantes

