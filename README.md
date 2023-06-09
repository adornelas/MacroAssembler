# AssemblerLinker

Equipe:

- André Dornelas Sanches - 17/0099369
- Maria Claudia Campos Martins - 17/0109968

O projeto AssemblerLinker é um trabalho feito para a disciplina de Software Básico (CIC0104) na Universidade de Brasília. 

Seu objetivo principal é montar e ligar arquivos escritos em "assembly inventado" dado na disciplina, seguindo uma série de requisitos detalhados no roteiro do Trabalho 1 disponibilizado pelo professor Bruno Luigi. 

O projeto foi feito no Unix, mais especificadamente, Ubuntu, utilizando a linguagem C++ (versão c++ 20). A versão do g++ utilizada foi a 11.3.0.

## Como compilar

Para compilar, basta digitar no terminal 

```
make
```

Esse comando irá executar **todos** os comandos necessários (tanto o montador, quanto o ligador) para o projeto, de acordo com o que foi definido na Makefile. 

### Alternativa para compilação 

Caso a versão do g++ seja diferente, pode ser que o programa não seja compilado. Assim, podem ser usados os seguintes comandos:

#### Para compilar o montador:

```
g++ montador.cpp src/*.cpp -o montador -std=c++20
```

#### Para compilar o ligador:

```
g++ linker.cpp src/*.cpp -o linker -std=c++20
```

## Como executar 

Para executar os códigos, basta digitar no terminal as seguintes linhas de comando.

#### Para executar o montador:

```
./montador <nome_programa> 
```

##### Notas sobre o montador: 

O montador aceita somente o nome do programa em assembly (com formato .asm). Além disso, podem ser passados até 4 arquivos. 

Os arquivos gerados pelo montador são os arquivos pré-processados, com nome no formato ```preprocessed_<nome_progama>.asm```, assim como os arquivos objetos no formato ```<nome_programa>.obj```. Quando apenas um arquivo é passado pelo terminal, a saída do montador é um arquivo no formato ```<nome_programa>.exc```.

Se apenas um arquivo for enviado para o montador e ele possuir dependências a serem resolvidas, não ocorrerá erro. A saída será no formato ```.exc``` contendo apenas a tabela CODE que seria gerada na ligação.

#### Para executar o ligador 

```
./linker <nome_programa> 
```

##### Notas sobre o ligador:

Vale notar que o ligador deve ser executado após o montador, pois recebe como entrada os arquivos objetos. Assim, devem ser enviados arquivos no formato ```<nome_programa>.obj``` gerados na saída do montador.

A saída do ligador será apenas um arquivo no formato ```.exc``` com o nome do primeiro arquivo objeto passado como argumento pelo terminal. 


## Sobre a identificação dos erros 

Os seguintes erros definidos no roteiro do trabalho estão sendo identificados e classificados como: 

- Dois rotulos na mesma linha - SEMANTICO
- Rótulo não definido na seção de TEXT - SEMÂNTICO
- Dado não definido na seção DATA - SEMÂNTICO
- Seção TEXT faltante - SEMÂNTICO
- EXTERN e PUBLIC em arquivo que NAO tenha BEGIN e END - SEMÂNTICO
- Erros léxicos (caracteres especiais fora o “_” (underscore) ou “numero inicial nos rótulos) - LÉXICO

Outros erros foram identificados e classificados pois faziam parte do algoritmo do montador ou ligador. Entretanto, como não faziam parte dos requisitos do trabalho, não serão citados neste texto. 

Vale notar que o arquivo onde os erros estão sendo indicados (com o valor da linha no arquivo) é o arquivo **pré-processado**.

## Mais observações 

### Uso do rótulo no operando de SPACE com argumentos

O uso de operações de soma e subtração com rótulos definidos através do SPACE foi implementado somente no montador. Na ligação, o resultado será diferente do esperado nos casos em que houver uma linha utilizando essa operação. 

Além disso, para os testes sempre usar espaço como

```
X + 2 
```

e não 

```
X+2 
```

Pois pode gerar erros e o programa não será processado corretamente. 

Não foi verificado se isso acontece com ```X:SPACE``` ou qualquer outro tipo de argumento no rotulo de X. Então, a principio, qualquer rótulo no operando pode aceitar operação com '+' ou '-'.

