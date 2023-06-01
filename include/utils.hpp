#ifndef UTILS_H
#define UTILS_H

#include "datatypes.hpp"

bool isLabel(std::string str);
void ConvertFileToMatrix(fileData * input_file, tokenMatrix * input_matrix);
void ConvertFileToMatrixCaps(fileData * input_file, tokenMatrix * input_matrix);
void ConvertMatrixToFile(tokenMatrix * output_matrix, fileData * output_file);

std::vector <std::string> Split(std::string text, char separator);
std::vector <std::string> CommonSplit(std::string text, char separator);

#endif