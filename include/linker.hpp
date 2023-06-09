#ifndef LINKER_H
#define LINKER_H

#include "datatypes.hpp"
#include "utils.hpp"

void Link(std::vector<tokenMatrix> &input_matrixes, fileData *output_file);
void SeparateMatrixes(std::vector<tokenMatrix> &input_matrixes, std::vector<objectData> &modules);

#endif