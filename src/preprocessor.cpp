#include "../include/preprocessor.hpp"

void PreProcessing(fileData *input_file, fileData *outuput_file)
{
    tokenMatrix *input_matrix = new tokenMatrix{.lines = 0};
    tokenMatrix *output_matrix = new tokenMatrix{.lines = 0};

    ConvertFileToMatrixCaps(input_file, input_matrix);
    ConvertMatrixToFile(input_matrix, outuput_file);
}