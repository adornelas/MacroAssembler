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

void TranslateAssemblyToObject(tokenMatrix *input_matrix){
    std::vector<std::string> matrix_line;
    std::vector<symbolData> symbol_table;
    int symbol_address;

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        for(int j = 0; j < matrix_line.size(); j++){

            if(isLabel(matrix_line[j])){
                if(isSymbolOnSymbolTable(symbol_table, matrix_line[0], symbol_address)){
                    if(isSymbolDefined(symbol_table, matrix_line[0], symbol_address)){
                        printf("Erro: rotulo duplicado");
                    }

                    symbol_table[symbol_address].is_defined = true;
                    
                    // #TODO: substituir o valor de acordo com a lista de dependencias
                }

                insertOnSymbolTable(symbol_table, {matrix_line[j], j, true});
            }

            if(!isInstructionOrDirective(matrix_line[j]) && !isNumber(matrix_line[j])){
                if(isSymbolOnSymbolTable(symbol_table, matrix_line[0], symbol_address)){
                    if(isSymbolDefined(symbol_table, matrix_line[0], symbol_address)){
                        // #TODO: substituir valor do simbolo no codigo
                    }
                        
                    insertOnListOfDependecies(symbol_table, symbol_address, j);
                }

                insertOnSymbolTable(symbol_table, {matrix_line[j], j, false});

            }

            // aqui ele pode ser operando, diretiva ou numero 
        }

    }
}
