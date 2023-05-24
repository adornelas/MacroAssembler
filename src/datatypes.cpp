#include "../include/datatypes.hpp"

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