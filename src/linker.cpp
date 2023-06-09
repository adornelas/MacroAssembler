#include "../include/linker.hpp"

void Link(std::vector<tokenMatrix> &input_matrixes, fileData *output_file){
    std::vector<objectData> modules(input_matrixes.size());
    std::map<int, int> correction_factor;   // <index, correction_factor>
    std::vector<symbolData> global_definition_table;


    SeparateMatrixes(input_matrixes, modules);

    if(input_matrixes.size() == 1){
        ConvertArrayObjectToFile(modules.begin()->assembled_code, output_file);
    }
    else{
        int accumulated_module_size = 0;
        std::string text;

        for(auto current_module : modules){
            // insere fator de correção de cada um dos módulos
            if(current_module.index != 0){
                correction_factor.insert(correction_factor.end(), {current_module.index, accumulated_module_size});
            }
            else{
                correction_factor.insert(correction_factor.end(), {current_module.index, 0});
            }

            for(auto current_symbol : current_module.definition_table){
                // insere os símbolos de cada uma das tabelas de definição na tabela global de definições
                insertOnSymbolTable(global_definition_table, {.name = current_symbol.name, .value = current_symbol.value, .module_index = current_module.index});
            }
            accumulated_module_size += current_module.assembled_code.size();
        }

        // atualiza os valores da tabela global de definições a partir do fator de correção
        for(int i = 0; i < global_definition_table.size(); i++){
            for(auto correction : correction_factor){
                if(correction.first == global_definition_table[i].module_index){
                    global_definition_table[i].value += correction.second;
                }
            }
        }    

        for(int i = 0; i < modules.size(); i++)
        {
            objectData current_module = modules[i];
            int symbol_address;

            // Corrigir os endereços das entradas da tabela de uso, utilizando a tabela global de definições
            for(int j = 0; j < modules[i].use_table.size(); j++){  
                symbol_address = isSymbolOnSymbolTable(global_definition_table, modules[i].use_table[j].name);
                modules[i].assembled_code.at(modules[i].use_table[j].value) = std::to_string(global_definition_table[symbol_address].value);
            }
            
            // Corrigir os endereços relativos usando os fatores de correção
            for(int j = 0; j < modules[i].relative_table.size(); j++){
                int posicaoEmAssembledCode = std::stoi(modules[i].relative_table[j]);
                int valorEmAssembledCode = std::stoi(modules[i].assembled_code.at(std::stoi(modules[i].relative_table[j])));
                int fatordecorrecao = correction_factor.at(i);
                modules[i].assembled_code.at(std::stoi(modules[i].relative_table[j])) = std::to_string(std::stoi(modules[i].assembled_code.at(std::stoi(modules[i].relative_table[j]))) + correction_factor.at(i));
            }

        }

        // Gerar código executável e salvar em arquivo
        for(int i = 0; i < modules.size(); i++){
            for(int j = 0; j < modules[i].assembled_code.size(); j++){
                text.append(modules[i].assembled_code[j]);
                text.append(" ");
            }
        }
        text.pop_back();
        output_file->content = text;
    }
}

void SeparateMatrixes(std::vector<tokenMatrix> &input_matrixes, std::vector<objectData> &modules){
    std::vector<std::string> matrix_line;
    tokenMatrix current_matrix;
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
            else if(matrix_line.size() == 1 && matrix_line[0].compare("DEF") == 0){
                useSection = false;
                defSection = true;
            }
            else if(matrix_line.size() == 1 && matrix_line[0].compare("RELATIVOS") == 0){
                defSection = false;
                relSection = true;
            }
            else if(matrix_line.size() == 1 && matrix_line[0].compare("CODE") == 0){
                relSection = false;
                codeSection = true;
            }
            else if(useSection){
                use_table.insert(use_table.end(), {.name = matrix_line[0],.value = std::stoi(matrix_line[1])});
            }
            else if(defSection){
                definition_table.insert(definition_table.end(), {.name = matrix_line[0],.value = std::stoi(matrix_line[1])});
            }
            else if(relSection){
                for (int j = 0; j < matrix_line.size(); j++){
                    relative_table.insert(relative_table.end(), matrix_line[j]);                    
                }
            }
            else if(codeSection){
                for (int j = 0; j < matrix_line.size(); j++){
                    assembled_code.insert(assembled_code.end(), matrix_line[j]);                    
                }
            }

        }

        modules.at(index).index = index;
        modules.at(index).use_table = use_table;
        modules.at(index).definition_table = definition_table;
        modules.at(index).relative_table = relative_table;
        modules.at(index).assembled_code = assembled_code;
        
        index++;
    }
}