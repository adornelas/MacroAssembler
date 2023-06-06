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
    if (argc < 2 && argc > 5) {
        cout << "Quantidade de argumentos invalida (" << argc << "),\ninsira até 4 arquivos no programa: nomedoprograma1.asm nomedoprograma2.asm nomedoprograma3.asm nomedoprograma4.asm" << endl;
        return 1;
    }
    else if(argc == 2) {
        fileData *input_file = new fileData{.name = argv[1]};
        input_file->name.append(".asm");
        fileData *output_file_pre_processed = new fileData{.name = argv[1]};
        output_file_pre_processed->name.append("_preprocessed.asm");
        fileData *output_file_assembled = new fileData{.name = argv[1]};
        output_file_assembled->name.append(".exc");
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

        PreProcess(input_file, output_file_pre_processing);

        ofstream opfs(output_file_pre_processing->name);
        opfs << output_file_pre_processing->content;
        opfs.close();

        Assemble(output_file_pre_processing, output_file_assembler);

        ofstream ofs(output_file_assembler->name);
        ofs << output_file_assembler->content;
        ofs.close();

        delete input_file;
        delete output_file_pre_processing;
        delete output_file_assembler;
    }
    else{
        for (int i = 1; i < argc; i++) {
            fileData* input_file = new fileData{ .name = argv[i] };
            fileData* output_file_pre_processed = new fileData{ .name = argv[i] };
            fileData* output_file_assembled = new fileData{ .name = argv[i] };

            input_file->name.append(".asm");
            output_file_pre_processed->name.append("_preprocessed.asm");
            output_file_assembled->name.append(".obj");

            ifstream ifs(input_file->name);
            stringstream buffer;
            buffer << ifs.rdbuf();
            input_file->content = buffer.str();

            PreProcess(input_file, output_file_pre_processed);

            ofstream opfs(output_file_pre_processed->name);
            opfs << output_file_pre_processed->content;
            opfs.close();

            AssembleModule(output_file_pre_processed, output_file_assembled);

            ofstream ofs(output_file_assembled->name);
            ofs << output_file_assembled->content;
            ofs.close();

            delete input_file;
            delete output_file_pre_processed;
            delete output_file_assembled;
        }
    }
}