#include "include/datatypes.hpp"
#include "include/preprocessor.hpp"

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
    fileData *output_file = new fileData{.name = "preprocessedfile"};

    ifstream ifs(input_file->name);
    stringstream buffer;
    buffer << ifs.rdbuf();
    input_file->content = buffer.str();

    // #TODO: Verificar se nos arquivos testes existe algum caso com arquivo vazio
    if(input_file->content.length() <= 1){
        printf("Arquivo %s inválido!\n", input_file->name.c_str());
        return 1;
    }

    PreProcessing(input_file, output_file);

    ofstream ofs(output_file->name);
    ofs << output_file->content;
    ofs.close();

    // comentario explicativo: limpa objetos criados na memoria (palavra: new)
    delete input_file;
    delete output_file;
}