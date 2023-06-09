#include "../include/assembler.hpp"

void Assemble(fileData *input_file, fileData *output_file, bool hasToLink){
    
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    outputObj output_object;

    ConvertFileToMatrix(input_file, input_matrix);
    TranslateAssemblyToObject(input_file, input_matrix, output_object, hasToLink);
    
    if(hasToLink){
        ConvertModuleToFile(output_object, output_file);
        return;
    }

    ConvertArrayObjectToFile(output_object.assembled_code, output_file);

    delete input_matrix;
}

void TranslateAssemblyToObject(fileData *input_file, tokenMatrix *input_matrix, outputObj &output_object, bool hasToLink){
    std::vector<std::string> matrix_line;
    std::vector<symbolData> symbol_table;
    std::string symbol_clean_name; // usado para guardar o rotulo sem ':'

    int symbol_address = 0;
    int current_address = 0;
    int operand_quantity = 0; // usada para contar os operandos
    int value = 0; // usada para auxiliar na contagem da posição do operando
    int current_line_address = 0;
    int current_line_size = 0;

    std::vector<dependeciesData> list_aux; // usada para auxiliar na criação da lista de dependencias

    bool has_section_text = false;
    int line_label = 0;

    bool label_duplicated = false;
    int error_line_duplicated = 0;
    long int const_number = 0;
    std::string section = "TEXT";

    bool isInBegin = false;
    int didItEnd = 1;   // inicializa em 1 pra identificar erros
    std::vector<symbolData> use_table;
    std::vector<symbolData> definition_table;

    bool error_line_without_begin_end = false;
    bool error_lexic = false;

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];
        operand_quantity = 0;
        line_label = 0;
        error_line_without_begin_end = false;
        bool error_lexic = false;
        bool label_duplicated = false;

        for(int j = 0; j < matrix_line.size(); j++){
                        
            if(isLabel(matrix_line[j])){
                line_label++;
                symbol_clean_name = matrix_line[j];
                symbol_clean_name.erase(remove(symbol_clean_name.begin(), symbol_clean_name.end(), ':'), symbol_clean_name.end());
                
                if(hasLexicError(matrix_line[j])){
                    error_lexic = true;
                }

                if(symbol_clean_name.compare("EXTERN") == 0){

                    if(!isInBegin){
                        printf("[Arquivo %s] ERRO SEMÂNTICO: EXTERN sem BEGIN (linha %d)\n",input_file->name.c_str(), i + 1);
                    }
                    
                    if(isSymbolOnSymbolTable(symbol_table, matrix_line[j+1]) == -1){
                        insertOnTable(symbol_table, {.name = matrix_line[j+1],.value = 0,.is_valueRelative = false ,.is_defined = true ,.is_extern = true});
                        insertOnTable(use_table, {.name = matrix_line[j+1]});
                    } else{
                        printf("[Arquivo %s] ERRO SEMÂNTICO: rótulo duplicado (linha %d)\n", input_file->name.c_str(), i + 1);
                    }
                    j++;

                }
                else if(isSymbolOnSymbolTable(symbol_table, symbol_clean_name) != -1){
                    if(isSymbolDefined(symbol_table, symbol_clean_name)){
                        printf("[Arquivo %s] ERRO SEMÂNTICO: rótulo duplicado (linha %d)\n", input_file->name.c_str(), i + 1);
                    }
                    else{
                        symbol_address = isSymbolOnSymbolTable(symbol_table, symbol_clean_name);
                        symbol_table[symbol_address].is_defined = true;
                        symbol_table[symbol_address].value = current_line_address;
                    
                        for(int l = 0; l < symbol_table[symbol_address].list_of_dependencies.size(); l++){
                            output_object.assembled_code[symbol_table[symbol_address].list_of_dependencies[l].address] = std::to_string(symbol_table[symbol_address].value + symbol_table[symbol_address].list_of_dependencies[l].space_arg );
                        }

                        if(hasToLink){
                            symbol_address = isSymbolOnSymbolTable(definition_table, symbol_clean_name);
                            if(symbol_address != -1){
                                if(matrix_line[j+1].compare("CONST") == 0){
                                    definition_table[symbol_address].value = std::stoi(matrix_line[j+2]);
                                    output_object.definition_table.insert(output_object.definition_table.end(), definition_table[symbol_address]);
                                }
                                else {
                                    definition_table[symbol_address].value = current_line_address;
                                    output_object.definition_table.insert(output_object.definition_table.end(), definition_table[symbol_address]);
                                }
                            }
                        }
                    }
                }else{
                    if(hasToLink && matrix_line[j+1].compare("BEGIN") == 0){
                        insertOnTable(symbol_table, {.name = symbol_clean_name,.value =  current_line_address, .is_defined= true, .is_begin = true, .line = (i+1)});
                    }
                    else{
                        insertOnTable(symbol_table, {.name = symbol_clean_name,.value =  current_line_address, .is_defined= true, .line = (i+1)});
                    }
                }                
            }
            else if(isHeader(matrix_line[j])){

                if(matrix_line[j].compare("BEGIN") == 0){
                    isInBegin = true;
                    didItEnd = 0;
                }
                else if(matrix_line[j].compare("PUBLIC") == 0){
                    if(!isInBegin){
                        printf("[Arquivo %s] ERRO SEMÂNTICO: PUBLIC sem BEGIN (linha %d)\n",input_file->name.c_str(), i + 1);
                    }

                    if(isSymbolOnSymbolTable(symbol_table, matrix_line[j+1]) == -1){
                        insertOnTable(symbol_table, {.name = matrix_line[j+1],.is_defined = false});
                    }
                    else {
                        symbol_address = isSymbolOnSymbolTable(symbol_table, matrix_line[j+1]);
                        if(symbol_table[symbol_address].is_begin == true){
                            output_object.definition_table.insert(output_object.definition_table.end(), symbol_table[symbol_address]);
                        }
                    }

                    if(isSymbolOnSymbolTable(definition_table, matrix_line[j+1]) == -1){
                        insertOnTable(definition_table, {.name = matrix_line[j+1]});
                    }
                    else {
                        printf("[Arquivo %s] ERRO SEMÂNTICO: PUBLIC com mesmo simbolo (linha %d)\n",input_file->name.c_str(), i + 1);
                    }

                    j++;
                }
            }
            else if(isOperator(matrix_line[j])){ 
                operand_quantity++;
                value = current_line_address + operand_quantity;

                if(hasLexicError(matrix_line[j])){
                    error_lexic = true;
                }
                
                if(isSymbolOnSymbolTable(symbol_table, matrix_line[j]) != -1){
                    symbol_address = isSymbolOnSymbolTable(symbol_table, matrix_line[j]);
                    if(isSymbolDefined(symbol_table, matrix_line[j])){

                        dependeciesData auxData = {.address = value, .space_arg = 0};
                        if(j + 1 < matrix_line.size() && j + 2 < matrix_line.size()){
                            if(matrix_line[j+1] == "-" || matrix_line[j+1] == "+"){
                                if(isNumber(matrix_line[j+2])){
                                    std::string auxString = matrix_line[j+1] + matrix_line[j+2];
                                    long int aux_space_arg = strtol(auxString.c_str(), nullptr, 10);
                                    auxData.space_arg = aux_space_arg;
                                }
                            }
                        }

                        output_object.assembled_code.insert(output_object.assembled_code.end(), std::to_string(symbol_table[symbol_address].value + auxData.space_arg));

                        if(hasToLink){
                            symbol_address = isSymbolOnSymbolTable(use_table, matrix_line[j]);
                            if(isSymbolExtern(symbol_table, matrix_line[j])){
                                use_table[symbol_address].value = value;
                                output_object.use_table.insert(output_object.use_table.end(), use_table[symbol_address]);
                            }
                        }                    
                    }
                    else {
                        
                        dependeciesData auxData = {.address = value, .space_arg = 0};
                        if((j + 1) < matrix_line.size() && (j + 2) < matrix_line.size()){
                            if(matrix_line[j+1] == "-" || matrix_line[j+1] == "+"){
                                if(isNumber(matrix_line[j+2])){
                                    std::string auxString = matrix_line[j+1] + matrix_line[j+2];
                                    long int aux_space_arg = strtol(auxString.c_str(), nullptr, 10);
                                    auxData.space_arg = aux_space_arg;
                                }
                            }
                        }

                        insertOnListOfDependecies(symbol_table, symbol_address, auxData);
                        output_object.assembled_code.insert(output_object.assembled_code.end(), matrix_line[j]);                        
                    }
                }
                else {
                    list_aux.clear();

                    dependeciesData auxData = {.address = value, .space_arg = 0};
                    if(j + 1 < matrix_line.size() && j + 2 < matrix_line.size()){
                        if(matrix_line[j+1] == "-" || matrix_line[j+1] == "+"){
                            if(isNumber(matrix_line[j+2])){
                                std::string auxString = matrix_line[j+1] + matrix_line[j+2];
                                long int aux_space_arg = strtol(auxString.c_str(), nullptr, 10);
                                auxData.space_arg = aux_space_arg;
                            }
                        }
                    }

                    list_aux.insert(list_aux.end(), auxData);
                    insertOnTable(symbol_table, {.name = matrix_line[j],.value =  -1, .is_defined= false,.list_of_dependencies = list_aux, .line = (i+1), .section=section});
                    output_object.assembled_code.insert(output_object.assembled_code.end(), matrix_line[j]);
                }

                if(hasToLink) {
                    if(isSymbolOnSymbolTable(definition_table, matrix_line[j]) != -1){
                        output_object.relative_table.insert(output_object.relative_table.end(), std::to_string(value));
                    }
                }
            }
            else if(isInstructionOrDirective(matrix_line[j])){
                current_line_size += op_size_map.find(matrix_line[j])->second;;

                if((matrix_line[j].compare("CONST") != 0) && (matrix_line[j].compare("SPACE") != 0) && (matrix_line[j].compare("END") != 0)){
                    output_object.assembled_code.insert(output_object.assembled_code.end(), op_code_map.find(matrix_line[j])->second);
                }

                if(matrix_line[j].compare("END") == 0) {
                    if(isInBegin){
                        didItEnd = 1;
                        isInBegin = false;
                    } else {
                        didItEnd = -1;
                    }
                }  
                else if((matrix_line[j].compare("CONST") == 0)){
                    if(matrix_line.size() > j + 1) {
                        if(isLabel(matrix_line[j+1])){
                            error_line_duplicated = true;
                        }else{
                            if(matrix_line[j+1].starts_with("-0X")){
                                printf("[Arquivo %s] ERRO SINTÁTICO: diretiva CONST não aceita número negativo em hexadecimal (linha %d)\n",input_file->name.c_str(), i + 1 );
                            }
                            if(isNumber(matrix_line[j+1])){
                                const_number = toNumber(matrix_line[j+1]);
                                output_object.assembled_code.insert(output_object.assembled_code.end(), matrix_line[j+1]);
                            }
                        }
                    }
                }

                if((matrix_line[j].compare("SPACE") == 0)){
                    if(matrix_line.size() > j + 1) {
                        if(isLabel(matrix_line[j+1])){
                            error_line_duplicated = true;
                        }else{
                            for(int m = 0; m < stol(matrix_line[j+1]); m++){
                                output_object.assembled_code.insert(output_object.assembled_code.end(), "0");
                            }
                        }
                    }
                    else{
                        output_object.assembled_code.insert(output_object.assembled_code.end(), "0");
                    }
                }
            }

            else{
                if(matrix_line[j] == "TEXT"){
                    section = "TEXT";
                    has_section_text = true;
                }else if(matrix_line[j] == "DATA"){
                    section = "DATA";
                }
            }
        }

        current_line_address += current_line_size;
        current_line_size = 0;

        if(line_label > 1){
            printf("[Arquivo %s] ERRO SEMÂNTICO: dois rótulos na mesma linha (linha %d)\n",input_file->name.c_str(), i + 1 );
        }

        if(error_line_without_begin_end){
            printf("[Arquivo %s] ERRO SEMÂNTICO: EXTERN ou PUBLIC declarados em um arquivo sem BEGIN e END (linha %d)\n",input_file->name.c_str(),i+1);
        }

        if(error_lexic){
             printf("[Arquivo %s] ERRO LÉXICO: tokens não podem começar com digitos ou underscore (linha %d)\n",input_file->name.c_str(), i + 1);
        }

    }

    if(!has_section_text){
        printf("[Arquivo %s] ERRO SINTÁTICO: sem declaração de SECTION TEXT\n",input_file->name.c_str());
    }

    if(didItEnd ==  0){
        printf("[Arquivo %s] ERRO SEMÂNTICO: END não encontrado após BEGIN (linha %ld)\n",input_file->name.c_str(), input_matrix->matrix.size());
    } else if(didItEnd == -1){
        printf("[Arquivo %s] ERRO SEMÂNTICO: END sem declaração de BEGIN (linha %ld)\n",input_file->name.c_str(), input_matrix->matrix.size());
    }

    for(int i = 0; i < symbol_table.size(); i++){
        if(!symbol_table[i].is_defined){            
            if(symbol_table[i].section == "DATA"){
                printf("[Arquivo %s] ERRO SEMÂNTICO: dado indefinido(linha %d)\n ",input_file->name.c_str(), symbol_table[i].line);
            }
            if(symbol_table[i].section == "TEXT"){
                printf("[Arquivo %s] ERRO SEMÂNTICO: rótulo indefinido(linha %d)\n ",input_file->name.c_str(), symbol_table[i].line);
            }            
        }
    }
}

