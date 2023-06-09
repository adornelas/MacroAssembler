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
    std::string name_pre_processed_file;

    if (argc < 2 || argc > 5) {
        cout << "Quantidade de argumentos invalida (" << argc << "),\ninsira até 4 arquivos no programa: nomedoprograma1.asm nomedoprograma2.asm nomedoprograma3.asm nomedoprograma4.asm" << endl;
        return 1;
    }

    for(int i = 0; i < argc - 1 ; i++){
        fileData *input_file = new fileData{.name = argv[i+1]};
        input_file->name.append(".asm");

        name_pre_processed_file = argv[i+1];
        name_pre_processed_file = "preprocessed_" + name_pre_processed_file;
        fileData *output_file_pre_processed = new fileData{name_pre_processed_file};
        output_file_pre_processed->name.append(".asm");

        ifstream ifs(input_file->name);
        stringstream buffer;
        buffer << ifs.rdbuf();
        input_file->content = buffer.str();

        // #TODO: Verificar se nos arquivos testes existe algum caso com arquivo vazio
        if(input_file->content.length() <= 1){
            printf("Arquivo %s inválido!\n", input_file->name.c_str());
            return 1;
        }

        PreProcess(input_file, output_file_pre_processed);

        ofstream opfs(output_file_pre_processed->name);
        opfs << output_file_pre_processed->content;
        opfs.close();

        fileData *output_file_assembled = new fileData{.name = argv[i+1]};

        if( argc == 2 ){
            output_file_assembled->name.append(".exc");
            Assemble(output_file_pre_processed, output_file_assembled, false);
            argc--;
        }
        else{
            output_file_assembled->name.append(".obj");
            Assemble(output_file_pre_processed, output_file_assembled, true);
        }

        ofstream ofs(output_file_assembled->name);
        ofs << output_file_assembled->content;
        ofs.close();

        delete output_file_assembled;
        delete input_file;
        delete output_file_pre_processed;

    }
}
