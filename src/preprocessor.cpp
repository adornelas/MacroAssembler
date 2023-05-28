#include "../include/preprocessor.hpp"

// #TODO: Colocar a SECTION DATA sempre ao final do código pré processado
// #TODO: Lidar com diretiva CONST (verificar se está no formato correto e passar para decimal)
void PreProcessing(fileData *input_file, fileData *outuput_file)
{
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    tokenMatrix *output_matrix = new tokenMatrix{.lines = 0};

    ConvertFileToMatrixCaps(input_file, input_matrix);
    CleanMatrix(input_matrix);
    ConvertMatrixToFile(input_matrix, outuput_file);

    delete input_matrix;
    delete output_matrix;
}

bool isLabel(std::string str){
    for( int i = 0 ; i < str.size() ; i++){
        if(str[i] == ':'){
            return true;
        }
    }

    return false;
}

void CleanMatrix(tokenMatrix * input_matrix){
    std::vector<std::vector<std::string>> matrix;
    std::vector<std::string> matrix_line;
    std::vector<std::string> clean_line;

    for(int i = 0; i < input_matrix->lines; i++){
        matrix_line = input_matrix->matrix[i];
        if(matrix_line.size() == 0){
            input_matrix->matrix.erase(input_matrix->matrix.begin() + i);
            input_matrix->lines--;
            i--;
        }
    }

    for (int i = 0; i < input_matrix->lines; i++){
        matrix_line = input_matrix->matrix[i];
        if(matrix_line.size() == 1 && isLabel(matrix_line[0])){
            matrix_line.insert(matrix_line.end(), input_matrix->matrix[i+1].begin(), input_matrix->matrix[i+1].end());
            input_matrix->matrix.erase(input_matrix->matrix.begin() + i + 1);
        }
        clean_line = {};
        for (long unsigned int j = 0; j < matrix_line.size(); j++){
            if (matrix_line[j][0] == ';')
                break;
            
            clean_line.push_back(matrix_line[j]);
        }
        matrix.push_back(clean_line);
    }
    input_matrix->matrix = matrix;
}