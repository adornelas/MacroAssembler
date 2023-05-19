# MacroAssembler
Repo for CIC0104 - Software Básico project

Equipe:
* André Dornelas Sanches - 17/0099369
* Maria Cláudia - 17/

Observação: os programas foram feitos para linux (Ubuntu), mas o montador foi feito em C++

## Como compilar

## Como usar o montador

Para montar o programa, é preciso digitar:
```
g++ montador.cpp src/*.cpp -o montador -std=c++11
```

### Para tradução do código

```
./montador meuprograma.asm saida.obj
```
O resultado do pré-processamento é apenas o arquivo com a retirada de espaços desnecessários e todo em maiúsculo.

A tradução ainda não está implementada, apenas o pré-processamento.