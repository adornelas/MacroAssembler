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

    for(int i = 0; i < input_matrix->matrix.size(); i++){
        
    }
}
