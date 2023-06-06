#include "../include/preprocessor.hpp"

// #TODO: Lidar com diretiva CONST (verificar se está no formato correto e passar para decimal)
void PreProcess(fileData* input_file, fileData* output_file) {
    tokenMatrix* input_matrix = new tokenMatrix{ .lines = 0 };
    tokenMatrix* output_matrix = new tokenMatrix{ .lines = 0 };

    ConvertFileToMatrixCaps(input_file, input_matrix);
    CleanMatrix(input_matrix);
    ConvertMatrixToFile(input_matrix, output_file);

    delete input_matrix;
    delete output_matrix;
}

void CleanMatrix(tokenMatrix* input_matrix) {
    std::vector<std::string> matrix_line;
    std::vector<std::string> clean_line;
    std::vector<std::vector<std::string>> section_data;

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        clean_line = {};

        for (long unsigned int j = 0; j < matrix_line.size(); j++) {
            if (matrix_line[j][0] == ';')
                break;

            clean_line.push_back(matrix_line[j]);
        }
        input_matrix->matrix[i] = clean_line;
        matrix_line = input_matrix->matrix[i];

        if (matrix_line.size() == 0) {
            input_matrix->matrix.erase(input_matrix->matrix.begin() + i);
            input_matrix->lines--;
            i--;
        }
    }

    for (int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        if (matrix_line.size() == 1 && isLabel(matrix_line[0])) {
            matrix_line.insert(matrix_line.end(), input_matrix->matrix[i + 1].begin(), input_matrix->matrix[i + 1].end());
            input_matrix->matrix.erase(input_matrix->matrix.begin() + i + 1);
        }

    }

    bool isInSectionData = false;

    for (int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        if (isInSectionData) {
            for (int j = 0; j < input_matrix->matrix[i].size(); j++) {
                if (input_matrix->matrix[i][j] == "SECTION" && input_matrix->matrix[i][j + 1] == "TEXT") {
                    isInSectionData = false;
                    break;
                }
            }

            if (isInSectionData) {
                section_data.push_back(input_matrix->matrix[i]);
                input_matrix->matrix.erase(input_matrix->matrix.begin() + i);
                i--;
            }

        }
        else {
            for (int j = 0; j < input_matrix->matrix[i].size(); j++) {
                if (input_matrix->matrix[i][j] == "SECTION" && input_matrix->matrix[i][j + 1] == "DATA") {
                    section_data.push_back(input_matrix->matrix[i]);
                    input_matrix->matrix.erase(input_matrix->matrix.begin() + i);
                    i--;
                    isInSectionData = true;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < section_data.size(); i++) {
        input_matrix->matrix.push_back(section_data[i]);
    }

    for (int i = 0; i < input_matrix->matrix.size(); i++){
        matrix_line = input_matrix->matrix[i];

        for (int j = 0; j < input_matrix->matrix[i].size(); j++){
            if(input_matrix->matrix[i][j] == "CONST"){
                if(input_matrix->matrix[i][j+1].starts_with("-0X")) {
                    printf("ERRO - diretiva CONST não aceita número negativo em hexadecimal\n");
                } else if(input_matrix->matrix[i][j+1].starts_with("0X")){
                    long hexNumber = std::stol(input_matrix->matrix[i][j+1],nullptr,16);
                    input_matrix->matrix[i].pop_back();
                    input_matrix->matrix[i].push_back(std::to_string(hexNumber));
                }
            }
        }
    }

}