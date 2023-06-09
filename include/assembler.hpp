#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "assemblerData.hpp"
#include "datatypes.hpp"
#include "utils.hpp"

void Assemble(fileData *input_file, fileData *output_file, bool hasToLink);
void TranslateAssemblyToObject(fileData *input_file,tokenMatrix *input_matrix, outputObj &output_object, bool hasToLink);

#endif
