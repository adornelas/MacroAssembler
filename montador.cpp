#include "include/datatypes.hpp"
#include "include/preprocessor.hpp"
#include "include/assembler.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    std::string teste;
    if (argc != 2)
    {
        cout << "Quantidade de argumentos invalida (" << argc << "),\ninsira no programa -diretiva nomedoprograma.asm nomedasaida.obj" << endl;
        return 1;
    }

    fileData *input_file = new fileData{.name = argv[1]};
    input_file->name.append(".asm");
    fileData *output_file_pre_processing = new fileData{.name = "preprocessedFile"};
    fileData *output_file_assembler = new fileData{.name = "assembledFile"};

    ifstream ifs(input_file->name);
    stringstream buffer;
    buffer << ifs.rdbuf();
    input_file->content = buffer.str();

    // #TODO: Verificar se nos arquivos testes existe algum caso com arquivo vazio
    if(input_file->content.length() <= 1){
        printf("Arquivo %s inválido!\n", input_file->name.c_str());
        return 1;
    }

    PreProcessing(input_file, output_file_pre_processing);
    Assembler(output_file_pre_processing, output_file_assembler);

    ofstream opfs(output_file_pre_processing->name);
    opfs << output_file_pre_processing->content;
    opfs.close();

    ofstream ofs(output_file_assembler->name);
    ofs << output_file_assembler->content;
    ofs.close();

    delete input_file;
    delete output_file_pre_processing;
    delete output_file_assembler;
}