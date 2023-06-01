#include "../include/assembler.hpp"

void Assembler(fileData *input_file, fileData *output_file){
    
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    tokenMatrix *output_matrix = new tokenMatrix{.lines = 0};

    ConvertFileToMatrix(input_file, input_matrix);
    TranslateAssemblyToObject(input_matrix);
    ConvertMatrixToFile(input_matrix, output_file);

    delete input_matrix;
    delete output_matrix;
}

bool isSymbolOnSymbolTable(std::vector<symbolData> symbol_table, std::string symbol,  int symbol_address){
    bool is_symbol_on_table = false; 

    for(int j = 0; j < symbol_table.size(); j++){
        if(symbol == symbol_table[j].name){
            is_symbol_on_table = true;
            symbol_address = j;
            break;
        }
    }

    return is_symbol_on_table;
}

bool isSymbolDefined(std::vector<symbolData> symbol_table, std::string symbol, int symbol_address){
    bool is_symbol_defined = false; 

    for(int j = 0; j < symbol_table.size(); j++){
        if(symbol == symbol_table[j].name){
            if(symbol_table[j].is_defined){
                is_symbol_defined = true;
                symbol_address = j;
            }
            break;
        }
    }

    return is_symbol_defined;
}

void isInstruction(std::string token){

}

void TranslateAssemblyToObject(tokenMatrix *input_matrix){
    std::vector<std::string> matrix_line;
    std::vector<symbolData> symbol_table;
    int symbol_address;

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        if(isLabel(matrix_line[0])){
            if(isSymbolOnSymbolTable(symbol_table, matrix_line[0], symbol_address)){
                if(isSymbolDefined(symbol_table, matrix_line[0], symbol_address)){
                    printf("Erro: rotulo duplicado");
                }

                if(!isSymbolDefined(symbol_table, matrix_line[0], symbol_address)){
                    symbol_table[symbol_address].is_defined = true;
                    
                    // #TODO: substituir o valor de acordo com a lista de dependencias
                }
            }

            symbol_table.insert(symbol_table.end(), {matrix_line[0], i, .is_defined = true});
            if (isLabel(matrix_line[1]))
                printf("Erro: dois rotulos na mesma linha");
        }


        // nao é rotulo
            // se nao é instrucao nem diretiva é operando
            // se operando nao é numero
                // se acha operando na TS
                    // se é indefinido
                        // coloca address na lista de dependencias
                    // se é definido
                        // substitui o valor do simbolo no codigo
                // se nao achou 
                    // coloca simbolo na TS 
    }
}
