#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "assemblerData.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

void Assembler(fileData *input_file, fileData *output_file);
void TranslateAssemblyToObject(fileData *input_file,tokenMatrix *input_matrix, std::vector<std::string> &output_object);

#endif
