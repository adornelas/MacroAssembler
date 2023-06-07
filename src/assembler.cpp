#include "../include/assembler.hpp"

// #TODO: SPACE pode aceitar argumento. Logo é possível fazer rótulos como X + 2 (com espaços)
// #TODO: O comando COPY deve separar os argumentos por “,” COM espaços antes e depois da “,”(ex: COPY A1 , A2)
void Assembler(fileData *input_file, fileData *output_file){
    
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    std::vector<std::string> output_object;

    ConvertFileToMatrix(input_file, input_matrix);
    TranslateAssemblyToObject(input_file, input_matrix, output_object);
    ConvertArrayObjectToFile(output_object, output_file);

    delete input_matrix;
}

void TranslateAssemblyToObject(fileData *input_file,tokenMatrix *input_matrix, std::vector<std::string> &output_object){
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
    bool has_section_text = false;

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
                        printf("[Arquivo %s] ERRO SEMÂNTICO: rótulo duplicado (linha %d)\n", input_file->name.c_str(), i + 1);
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
                    insertOnSymbolTable(symbol_table, {.name = symbol_clean_name,.value =  current_line_address, .is_defined= true, .line = (i+1)});
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
                    insertOnSymbolTable(symbol_table, {.name = matrix_line[j],.value =  -1, .is_defined= false,.list_of_dependencies = list_aux, .line = (i+1)});
                    output_object.insert(output_object.end(), matrix_line[j]);
                }
            }
            else if(isInstructionOrDirective(matrix_line[j])){
                current_line_size += op_size_map.find(matrix_line[j])->second;;

                if((matrix_line[j].compare("CONST") != 0) && (matrix_line[j].compare("SPACE") != 0)){
                    output_object.insert(output_object.end(), op_code_map.find(matrix_line[j])->second);
                }

                if((matrix_line[j].compare("CONST") == 0)){
                    if(matrix_line.size() > j + 1) {
                        output_object.insert(output_object.end(), matrix_line[j+1]);
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

            else{
                if(matrix_line[j] == "TEXT"){
                    has_section_text = true;
                }
            }
        }

        current_line_address += current_line_size;
        current_line_size = 0;

    }

    if(!has_section_text){
        printf("[Arquivo %s] ERRO SINTÁTICO: SECTION TEXT ausente\n",input_file->name.c_str());
    }

    bool symbol_found = false;
    int error_line = 0;
    for(int i = 0; i < symbol_table.size(); i++){
        if(!symbol_table[i].is_defined){
            symbol_found = true;
            error_line = symbol_table[i].line;
            break;
        }
    }

    if(symbol_found){
        printf("[Arquivo %s] ERRO SEMÂNTICO: simbolo indefinido (linha %d)\n ",input_file->name.c_str(), error_line);
    }
}
