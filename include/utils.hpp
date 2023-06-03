#ifndef UTILS_H
#define UTILS_H

#include "datatypes.hpp"
#include "assemblerData.hpp"

bool isLabel(std::string str);
void ConvertFileToMatrix(fileData * input_file, tokenMatrix * input_matrix);
void ConvertFileToMatrixCaps(fileData * input_file, tokenMatrix * input_matrix);
void ConvertMatrixToFile(tokenMatrix * output_matrix, fileData * output_file);

std::vector <std::string> Split(std::string text, char separator);
std::vector <std::string> CommonSplit(std::string text, char separator);

int isSymbolOnSymbolTable(std::vector<symbolData> &symbol_table, std::string symbol);
bool isSymbolDefined(std::vector<symbolData> &symbol_table, std::string symbol);
bool isInstructionOrDirective(std::string token);
bool isNumber(std::string token);
bool isOperator(std::string token);
void insertOnListOfDependecies(std::vector<symbolData> &symbol_table, int symbol_address, int token_address);
void insertOnSymbolTable(std::vector<symbolData> &symbol_table, symbolData symbol_data );

void ConvertArrayObjectToFile(std::vector<std::string> &output_object, fileData * output_file);
void ConvertModuleToFile(outputObj * output_object, fileData * output_file);

#endif