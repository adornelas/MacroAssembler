#include "include/datatypes.hpp"
#include "include/preprocessor.hpp"
#include "include/assembler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    vector<tokenMatrix> input_matrixes(argc - 1);

    if (argc < 2 && argc > 5) {
        cout << "Quantidade de argumentos invalida (" << argc << "),\ninsira até 4 arquivos no programa: nomedoprograma1 nomedoprograma2 nomedoprograma3 nomedoprograma4" << endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        fileData* input_file = new fileData{ .name = argv[i] };
        input_matrixes.push_back({ .lines = 0 });

        input_file->name.append(".obj");
        ifstream ifs(input_file->name);
        stringstream buffer;
        buffer << ifs.rdbuf();
        input_file->content = buffer.str();
        ConvertFileToMatrix(input_file, &input_matrixes.at(i));
        delete input_file;
    }

    fileData* output_file = new fileData{ .name = argv[1] };
    output_file->name.append(".exc");

    // ligador

    ofstream ofs(output_file->name);
    ofs << output_file->content;
    ofs.close();

    delete output_file;
}