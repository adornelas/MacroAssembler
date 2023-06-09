#include "../include/utils.hpp"
#include "../include/assemblerData.hpp"

bool isLabel(std::string str){
    for( int i = 0 ; i < str.size() ; i++){
        if(str[i] == ':'){
            return true;
        }
    }

    return false;
}

bool hasLexicError(std::string token){
    if(token[0] == '_' || isdigit(token[0])){
        return true;
    }

    return false;

}

void ConvertFileToMatrix(fileData * input_file, tokenMatrix * input_matrix){
    int lines = 0;
    std::string temp;
    std::vector<std::string> input_line;
    std::vector<std::string> matrix_line;

    input_line = CommonSplit(input_file->content, '\n');
    for (long unsigned int i = 0; i<input_line.size(); i++){
        matrix_line = CommonSplit(input_line[i], ' ');
        input_matrix->matrix.push_back(matrix_line);
        lines++;
    }
    input_matrix->lines = lines;
}

void ConvertFileToMatrixCaps(fileData * input_file, tokenMatrix * input_matrix){
    int lines = 0;
    std::string temp;
    std::vector<std::string> input_line;
    std::vector<std::string> matrix_line;

    input_line = Split(input_file->content, '\n');
    for (long unsigned int i = 0; i<input_line.size(); i++){
        matrix_line = Split(input_line[i], ' ');
        input_matrix->matrix.push_back(matrix_line);
        lines++;
    }
    input_matrix->lines = lines;
}

void ConvertArrayObjectToFile(std::vector<std::string> &output_object, fileData * output_file){
    std::string text;
    std::string matrix_line;
    for (int i = 0; i < output_object.size(); i++){
        text.append(output_object[i]);
        text.append(" ");
    }
    text.pop_back();
    output_file->content = text;
}

void ConvertModuleToFile(outputObj &output_object, fileData * output_file){
    std::string text;
    std::string matrix_line;
    
    text.append("USO\n");
    for (int i = 0; i < output_object.use_table.size(); i++){
        text.append(output_object.use_table[i].name);
        text.append(" ");
        text.append(std::to_string(output_object.use_table[i].value));
        text.append("\n");

    }

    text.append("DEF\n");
    for (int i = 0; i < output_object.definition_table.size(); i++){
        text.append(output_object.definition_table[i].name);
        text.append(" ");
        text.append(std::to_string(output_object.definition_table[i].value));
        text.append("\n");
    }

    text.append("RELATIVOS\n");
    for (int i = 0; i < output_object.relative_table.size(); i++){
        text.append(output_object.relative_table[i]);
        text.append(" ");
    }
    text.pop_back();
    text.append("\n");


    text.append("CODE\n");
    for (int i = 0; i < output_object.assembled_code.size(); i++){
        text.append(output_object.assembled_code[i]);
        text.append(" ");
    }
    text.pop_back();
    output_file->content = text;
}

void ConvertMatrixToFile(tokenMatrix * output_matrix, fileData * output_file){
    std::string text;
    std::vector <std::string> matrix_line;
    for (long unsigned int i = 0; i < output_matrix->matrix.size(); i++){
        matrix_line = output_matrix->matrix[i];
        if (matrix_line.size() == 0){
            text.append("\n");
            continue;    
        }
        for (long unsigned int j = 0; j < matrix_line.size(); j++){
            text.append(matrix_line[j]);
            text.append(" ");
        }
        text.pop_back();
        text.append("\n");
    }
    text.pop_back();
    output_file->content = text;
}

std::vector <std::string> Split(std::string text, char separator){
    std::string str;
    std::stringstream ss(text);
    std::vector <std::string> result;
    std::size_t id;
    while(getline(ss, str, separator)){
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        if (separator == ' '){
            id = str.find_first_not_of(' ');
            if (id != std::string::npos){
                str.substr(id, str.size());
                result.push_back(str);
            }
        } else {
            result.push_back(str);
        }
    }
    return result;
}

std::vector <std::string> CommonSplit(std::string text, char separator){
    std::string str;
    std::stringstream ss(text);
    std::vector <std::string> result;
    while(getline(ss, str, separator)){
        result.push_back(str);
    }
    return result;
}

int isSymbolOnSymbolTable(std::vector<symbolData> &symbol_table, std::string symbol){
    int symbol_address = -1;

    for(int j = 0; j < symbol_table.size(); j++){
        if(symbol.compare(symbol_table[j].name) == 0){
            symbol_address = j;
            break;
        }
    }

    return symbol_address;
}

bool isSymbolDefined(std::vector<symbolData> &symbol_table, std::string symbol){
    bool is_symbol_defined = false; 

    for(int j = 0; j < symbol_table.size(); j++){
        if(symbol == symbol_table[j].name){
            if(symbol_table[j].is_defined){
                is_symbol_defined = true;
                break;
            }
        }
    }

    return is_symbol_defined;
}

bool isSymbolExtern(std::vector<symbolData> &symbol_table, std::string symbol){
    bool is_symbol_extern = false; 

    for(int j = 0; j < symbol_table.size(); j++){
        if(symbol == symbol_table[j].name){
            if(symbol_table[j].is_extern){
                is_symbol_extern = true;
                break;
            }
        }
    }

    return is_symbol_extern;
}

bool isInstructionOrDirective(std::string token){
    
    if(op_size_map.find(token) == op_size_map.end()){
        return false;
    }

    return true;
}

bool isHeader(std::string token){
    bool found = false;

    if (auto search = op_type_map.find(token); search != op_type_map.end()){
        if(search->second == 'c')
        found = true;
    }

    return found;
} 

long int toNumber(std::string token){
    char *p;
    long int number;
    number = strtol(token.c_str(), &p, 10);
    if (*p == 0){
        return number;
    }

    number = strtol(token.c_str(), &p, 16);
    return number;
}

bool isNumber(std::string token){
    
    char *p;
    strtol(token.c_str(), &p, 10);
    if (*p == 0){
        return true;
    }

    strtol(token.c_str(), &p, 16);
    return *p == 0;
}

bool isOperator(std::string token){
    
    if((token.compare("SECTION") != 0) && (token.compare("TEXT") != 0) && (token.compare("DATA") != 0) && (token.compare(",") != 0)) {
        if(!isInstructionOrDirective(token) && !isNumber(token)){
            return true;
        }
    }

    return false;
}

void insertOnListOfDependecies(std::vector<symbolData> &symbol_table, int symbol_address, int token_address){
   symbol_table[symbol_address].list_of_dependencies.insert(symbol_table[symbol_address].list_of_dependencies.end(), token_address );
}

void insertOnTable(std::vector<symbolData> &table, symbolData symbol_data ){
    table.insert(table.end(), symbol_data);
}

