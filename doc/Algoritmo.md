- *Enquanto* arquivo fonte não chegou ao fim, *faça*:
    - Obtém uma linha do código fonte
    - Separa os elementos da linha: rótulo, operação, operandos
    - *Se* há rótulo:
        - Procura rótulo na TS (Tabela de Simbolos):
            - *Se* achou e está definido: Erro -> Rótulo redefinido
            - *Se* achou e não está definido:
                - Coloca o valor do rótulo na tabela igual a LC (Location counter), Definido = T e substitui os valores na lista daquele símbolo (retroativamente)
            - *Se* não achou: 
                - Incluir na TS com valor = LC, Definido = T e a Lista = -1
    - Procura operação na lista de instruções
        - *Se não* existe: Erro -> Operação não existe
        - *Se* existe: Traduz
    - *Se* operando é símbolo:
        - Procura operando na TS:
            - *Se não* existe:
                - Insere o símbolo na TS, coloca um valor SIMBÓLICO, Definito = F, Lista = [LC, -1]
            - *Se* existe e está definido:
                - Substitui o valor do símbolo que está na TS
            - *Se* existe e NÃO está definido:
                - Insere o LC na lista específica do símbolo e liga com os itens já existentes.
    - *Se* operando é número válido: insere no código objeto


Erros a serem identificados:
   - Dois rotulos na mesma linha
   - Rótulo não definido na seção de TEXT
   - Dado não definido na seção DATA
   - Seção TEXT faltante
   - EXTERN e PUBLIC em arquivo que NAO tenha BEGIN e END
   - Erros Iéxicos (caracteres especiais fora o “_” (underscore) ou “numero inicial nos
    rótulos)

-------