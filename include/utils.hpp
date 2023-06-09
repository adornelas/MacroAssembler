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

/// @brief Verifies if the given symbol is in the symbol table
/// @param symbol_table reference to the symbol table
/// @param symbol symbol to search for
/// @return -1 in case the symbol is not there, or the symbol address if it is
int isSymbolOnSymbolTable(std::vector<symbolData> &symbol_table, std::string symbol);
bool isSymbolDefined(std::vector<symbolData> &symbol_table, std::string symbol);
bool isSymbolExtern(std::vector<symbolData> &symbol_table, std::string symbol);
bool isInstructionOrDirective(std::string token);
bool isNumber(std::string token);
bool isOperator(std::string token);
bool isHeader(std::string token);

void insertOnListOfDependecies(std::vector<symbolData> &symbol_table, int symbol_address, int token_address);
void insertOnTable(std::vector<symbolData> &table, symbolData symbol_data );

void ConvertArrayObjectToFile(std::vector<std::string> &output_object, fileData * output_file);
void ConvertModuleToFile(outputObj &output_object, fileData * output_file);
long int toNumber(std::string token);
bool hasLexicError(std::string token);


#endif