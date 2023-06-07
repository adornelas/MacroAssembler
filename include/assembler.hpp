#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "assemblerData.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

void Assembler(fileData *input_file, fileData *output_file);
void TranslateAssemblyToObject(tokenMatrix *input_matrix, std::vector<std::string> &output_object);
void TranslateModuleToObject(tokenMatrix *input_matrix, outputObj *output_object);


#endif
