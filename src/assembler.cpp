#include "../include/assembler.hpp"

// #TODO: SPACE pode aceitar argumento. Logo é possível fazer rótulos como X + 2 (com espaços)
// #TODO: O comando COPY deve separar os argumentos por “,” COM espaços antes e depois da “,”(ex: COPY A1 , A2)
void Assembler(fileData *input_file, fileData *output_file){
    
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    outputObj *output_object = new outputObj{};

    ConvertFileToMatrix(input_file, input_matrix);
    TranslateModuleToObject(input_matrix, output_object);
    ConvertModuleToFile(output_object, output_file);

    delete input_matrix;
}

void TranslateAssemblyToObject(tokenMatrix *input_matrix, std::vector<std::string> &output_object){
    std::vector<std::string> matrix_line;
    std::vector<symbolData> symbol_table;
    std::string symbol_clean_name; // usado para guardar o rotulo sem ':'

    int symbol_address = 0;
    int current_address = 0;
    int operand_quantity = 0; // usada para contar os operandos
    int value = 0; // usada para auxiliar na contagem da posição do operando
    int current_line_address = 0;
    int current_line_size = 0;

    int opcode;

    std::vector<int> list_aux; // usada para auxiliar na criação da lista de dependencias

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];
        operand_quantity = 0;

        for(int j = 0; j < matrix_line.size(); j++){
                        
            if(isLabel(matrix_line[j])){
                symbol_clean_name = matrix_line[j];
                symbol_clean_name.erase(remove(symbol_clean_name.begin(), symbol_clean_name.end(), ':'), symbol_clean_name.end());

                if(isSymbolOnSymbolTable(symbol_table, symbol_clean_name) != -1){
                    if(isSymbolDefined(symbol_table, symbol_clean_name)){
                        printf("ERRO - rotulo duplicado\n");
                    }
                    else{
                        
                        symbol_address = isSymbolOnSymbolTable(symbol_table, symbol_clean_name);
                        symbol_table[symbol_address].is_defined = true;
                        symbol_table[symbol_address].value = current_line_address;
                    
                        for(int l = 0; l < symbol_table[symbol_address].list_of_dependencies.size(); l++){
                            output_object[symbol_table[symbol_address].list_of_dependencies[l]] = std::to_string(symbol_table[symbol_address].value);
                        }
                    }
                }else{
                    insertOnSymbolTable(symbol_table, {.name = symbol_clean_name,.value = current_line_address,.is_defined = true});
                }                
            }
            else if(isOperator(matrix_line[j])){ 
                operand_quantity++;
                value = current_line_address + operand_quantity;
                
                if(isSymbolOnSymbolTable(symbol_table, matrix_line[j]) != -1){
                    if(isSymbolDefined(symbol_table, matrix_line[j])){
                        symbol_address = isSymbolOnSymbolTable(symbol_table, symbol_clean_name);
                        output_object.insert(output_object.end(), std::to_string(symbol_table[symbol_address].value));
                    }
                    else {
                        symbol_address = isSymbolOnSymbolTable(symbol_table, matrix_line[j]);
                        insertOnListOfDependecies(symbol_table, symbol_address, value);
                        output_object.insert(output_object.end(), matrix_line[j]);
                    }
                }
                else {
                    list_aux.clear();
                    list_aux.insert(list_aux.end(), value);
                    insertOnSymbolTable(symbol_table, {.name = matrix_line[j],.value =  -1,.is_defined = false,.list_of_dependencies = list_aux});
                    output_object.insert(output_object.end(), "-1");
                }
            }
            else if(isInstructionOrDirective(matrix_line[j])){
                current_line_size += op_size_map.find(matrix_line[j])->second;;

                if((matrix_line[j].compare("CONST") != 0) && (matrix_line[j].compare("SPACE") != 0)){
                    opcode = stol(op_code_map.find(matrix_line[j])->second);
                    output_object.insert(output_object.end(), std::to_string(opcode));
                }

                if((matrix_line[j].compare("CONST") == 0)){
                    if(matrix_line.size() > j + 1) {
                        output_object.insert(output_object.end(), matrix_line[j+1]);
                    }
                    else{
                        printf("ERRO - CONST SEM VALOR");
                    }
                }

                if((matrix_line[j].compare("SPACE") == 0)){
                    if(matrix_line.size() > j + 1) {
                        for(int m = 0; m < stol(matrix_line[j+1]); m++){
                            output_object.insert(output_object.end(), "0");
                        }
                    }
                    else{
                        output_object.insert(output_object.end(), "0");
                    }
                }
            }
        }

        current_line_address += current_line_size;
        current_line_size = 0;

    }

    bool symbol_found = false;
    for(int i = 0; i < symbol_table.size(); i++){
        if(!symbol_table[i].is_defined){
            symbol_found = true;
            break;
        }
    }

    if(symbol_found){
        printf("ERRO - o simbolo nao é definido\n ");
    }
}

void TranslateModuleToObject(tokenMatrix *input_matrix, outputObj *output_object){
    std::vector<std::string> matrix_line;
    std::vector<symbolData> symbol_table;
    std::vector<symbolData> use_table;
    std::vector<symbolData> definition_table;
    std::string symbol_clean_name; // usado para guardar o rotulo sem ':'

    int symbol_address = 0;
    int current_address = 0;
    int operand_quantity = 0; // usada para contar os operandos
    int value = 0; // usada para auxiliar na contagem da posição do operando
    int current_line_address = 0;
    int current_line_size = 0;
    bool isInBegin = false;
    int didItEnd = 1;   // inicializa em 1 pra identificar erros

    int opcode;

    std::vector<int> list_aux; // usada para auxiliar na criação da lista de dependencias

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];
        operand_quantity = 0;


        for(int j = 0; j < matrix_line.size(); j++){
                        
            if(isLabel(matrix_line[j])){
                symbol_clean_name = matrix_line[j];
                symbol_clean_name.erase(remove(symbol_clean_name.begin(), symbol_clean_name.end(), ':'), symbol_clean_name.end());

                if(isSymbolOnSymbolTable(symbol_table, symbol_clean_name) != -1){
                    if(isSymbolDefined(symbol_table, symbol_clean_name)){
                        printf("ERRO - rotulo duplicado\n");
                    }else{
                        symbol_address = isSymbolOnSymbolTable(symbol_table, symbol_clean_name);
                        symbol_table[symbol_address].is_defined = true;
                        symbol_table[symbol_address].value = current_line_address;
                    
                        for(int l = 0; l < symbol_table[symbol_address].list_of_dependencies.size(); l++){
                            output_object->assembled_code[symbol_table[symbol_address].list_of_dependencies[l]] = std::to_string(symbol_table[symbol_address].value);
                        }

                        symbol_address = isSymbolOnSymbolTable(definition_table, symbol_clean_name);
                        if(symbol_address != -1){
                            definition_table[symbol_address].value = current_line_address;
                            output_object->definition_table.insert(output_object->definition_table.end(), definition_table[symbol_address]);
                        }
                    }
                }else{
                    insertOnSymbolTable(symbol_table, {.name = symbol_clean_name,.value = current_line_address,.is_defined = true});
                }                
            } else if(isHeader(matrix_line[j])){
                if(matrix_line[j].compare("BEGIN") == 0){
                    isInBegin = true;
                    didItEnd = 0;
                } else if(matrix_line[j].compare("EXTERN") == 0){
                    if(!isInBegin){
                        printf("ERRO - EXTERN sem BEGIN\n");
                    }
                    if(isSymbolOnSymbolTable(symbol_table, matrix_line[j+1]) == -1){
                        insertOnSymbolTable(symbol_table, {.name = matrix_line[j+1],.value = 0,.is_valueRelative = false ,.is_defined = true ,.is_extern = true});
                        insertOnSymbolTable(use_table, {.name = matrix_line[j+1]});
                    } else{
                        printf("ERRO - rotulo duplicado\n");
                    }
                    j++;
                } else if(matrix_line[j].compare("PUBLIC") == 0){
                    if(!isInBegin){
                        printf("ERRO - PUBLIC sem BEGIN\n");
                    }
                    if(isSymbolOnSymbolTable(symbol_table, matrix_line[j+1]) != -1){
                        printf("ERRO - rotulo duplicado\n");
                    } else {
                        insertOnSymbolTable(symbol_table, {.name = matrix_line[j+1],.is_defined = false});
                        insertOnSymbolTable(definition_table, {.name = matrix_line[j+1]});
                    }
                    j++;
                }
            } else if(isOperator(matrix_line[j])){
                operand_quantity++;
                value = current_line_address + operand_quantity;
                
                if(isSymbolOnSymbolTable(symbol_table, matrix_line[j]) != -1){
                    if(isSymbolDefined(symbol_table, matrix_line[j])){
                        symbol_address = isSymbolOnSymbolTable(symbol_table, matrix_line[j]);
                        output_object->assembled_code.insert(output_object->assembled_code.end(), std::to_string(symbol_table[symbol_address].value));
                        
                        symbol_address = isSymbolOnSymbolTable(use_table, matrix_line[j]);
                        if(isSymbolExtern(symbol_table, matrix_line[j])){
                            use_table[symbol_address].value = value;
                            output_object->use_table.insert(output_object->use_table.end(), use_table[symbol_address]);
                        }
                    }
                    else {
                        symbol_address = isSymbolOnSymbolTable(symbol_table, matrix_line[j]);
                        insertOnListOfDependecies(symbol_table, symbol_address, value);
                        output_object->assembled_code.insert(output_object->assembled_code.end(), matrix_line[j]);
                    }
                }
                else {
                    list_aux.clear();
                    list_aux.insert(list_aux.end(), value);
                    insertOnSymbolTable(symbol_table, {.name = matrix_line[j],.value =  -1,.is_defined = false,.list_of_dependencies = list_aux});
                    output_object->assembled_code.insert(output_object->assembled_code.end(), matrix_line[j]);
                }
            } else if(isInstructionOrDirective(matrix_line[j])){
                current_line_size += op_size_map.find(matrix_line[j])->second;;

                if((matrix_line[j].compare("CONST") != 0) && (matrix_line[j].compare("SPACE") != 0)  && (matrix_line[j].compare("END") != 0)){
                    opcode = stol(op_code_map.find(matrix_line[j])->second);
                    output_object->assembled_code.insert(output_object->assembled_code.end(), std::to_string(opcode));
                }

                if(matrix_line[j].compare("END") == 0) {
                    if(isInBegin){
                        didItEnd = 1;
                        isInBegin = false;
                    } else {
                        didItEnd = -1;
                    }
                }

                if((matrix_line[j].compare("CONST") == 0)){
                    if(matrix_line.size() > j + 1) {
                        output_object->assembled_code.insert(output_object->assembled_code.end(), matrix_line[j+1]);
                    }
                    else{
                        printf("ERRO - CONST SEM VALOR");
                    }
                }

                if((matrix_line[j].compare("SPACE") == 0)){
                    if(matrix_line.size() > j + 1) {
                        for(int m = 0; m < stol(matrix_line[j+1]); m++){
                            output_object->assembled_code.insert(output_object->assembled_code.end(), "0");
                        }
                    }
                    else{
                        output_object->assembled_code.insert(output_object->assembled_code.end(), "0");
                    }
                }
            }
        }

        current_line_address += current_line_size;
        current_line_size = 0;

    }

    bool symbol_found = false;
    for(int i = 0; i < symbol_table.size(); i++){
        if(!symbol_table[i].is_defined){
            symbol_found = true;
            break;
        }
    }

    if(symbol_found){
        printf("ERRO - o simbolo nao é definido\n");
    }
    if(didItEnd ==  0){
        printf("ERRO - END não encontrado após BEGIN\n");
    } else if(didItEnd == -1){
        printf("ERRO - END sem BEGIN\n");
    }

    // TODO: passa endereços para tabela de definições
    
}
