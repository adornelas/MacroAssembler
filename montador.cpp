#include "include/datatypes.hpp"
#include "include/preprocessor.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Quantidade de argumentos invalida (" << argc << "),\ninsira no programa -diretiva nomedoprograma.asm nomedasaida.obj" << endl;
        return 0;
    }

    fileData *input_file = new fileData{.name = argv[1]};
    fileData *output_file = new fileData{.name = "preprocessedfile"};

    ifstream ifs(argv[1]);
    stringstream buffer;
    buffer << ifs.rdbuf();
    input_file->content = buffer.str();

    PreProcessing(input_file, output_file);

    ofstream ofs(output_file->name);
    ofs << output_file->content;
    ofs.close();
}