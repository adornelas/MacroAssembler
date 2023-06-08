#include "../include/linker.hpp"

void Link(std::vector<tokenMatrix> &input_matrixes, fileData *output_file){

    // Separar matrizes

    if(input_matrixes.size() == 1){
        // Apenas colocar no código de saída
    }
    else{
        // Ligar
    }
}

void SeparateMatrixes(std::vector<tokenMatrix> &input_matrixes, std::vector<objectData> &modules){
    std::vector<std::string> matrix_line;
    std::vector<std::string> next_matrix_line;
    int index = 0;

    for(auto current_matrix : input_matrixes){
        std::vector<symbolData> use_table;
        std::vector<symbolData> definition_table;
        std::vector<std::string> relative_table;
        std::vector<std::string> assembled_code;
        bool useSection = false;
        bool defSection = false;
        bool relSection = false;
        bool codeSection = false;

        for (int i = 0; i < current_matrix.matrix.size(); i++){
            matrix_line = current_matrix.matrix[i];

            if(matrix_line.size() == 1 && matrix_line[0].compare("USO") == 0){
                useSection = true;
            }
            else if(useSection){
                use_table.insert(use_table.end(), {.name = matrix_line[0],.value = std::stoi(matrix_line[1])});
            }
            else if(matrix_line.size() == 1 && matrix_line[0].compare("DEF") == 0){
                useSection = false;
                defSection = true;
            }else if(defSection){
                definition_table.insert(definition_table.end(), {.name = matrix_line[0],.value = std::stoi(matrix_line[1])});
            }
            else if(matrix_line.size() == 1 && matrix_line[0].compare("REL") == 0){
                defSection = false;
                relSection = true;
            }else if(relSection){
                for (int j = 0; j < matrix_line.size(); j++){
                    relative_table.insert(relative_table.end(), matrix_line[j]);                    
                }
            }
            else if(matrix_line.size() == 1 && matrix_line[0].compare("CODE") == 0){
                relSection = false;
                codeSection = true;
            }else if(codeSection){
                for (int j = 0; j < matrix_line.size(); j++){
                    assembled_code.insert(assembled_code.end(), matrix_line[j]);                    
                }
            }

        }

        modules.at(index).use_table = use_table;
        modules.at(index).definition_table = definition_table;
        modules.at(index).relative_table = relative_table;
        modules.at(index).assembled_code = assembled_code;
        
        index++;
    }
}