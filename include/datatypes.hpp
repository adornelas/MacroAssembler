#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

struct fileData {
    std::string name;
    std::string content;
};

struct tokenMatrix {
    int lines;
    std::vector<std::vector<std::string>> matrix;
};

struct equIdentifier {
    std::string name;
    int value;
};

void ConvertFileToMatrix(fileData * input_file, tokenMatrix * input_matrix);
void ConvertFileToMatrixCaps(fileData * input_file, tokenMatrix * input_matrix);
void ConvertMatrixToFile(tokenMatrix * output_matrix, fileData * output_file);

std::vector <std::string> Split(std::string text, char separator);
std::vector <std::string> CommonSplit(std::string text, char separator);

#endif