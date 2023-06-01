#include "../include/assemblerData.hpp"

std::map<std::string, char> op_type_map = {
    // 't' para operação da seção texto
    // 'd' para operação da seção dados
    // 'c' para operação do cabeçalho
    // Instruções
    {"ADD", 't'}, {"SUB", 't'}, {"MUL", 't'}, {"DIV", 't'}, {"JMP", 't'}, {"JMPN", 't'}, {"JMPP", 't'},{"JMPZ", 't'},
    {"COPY", 't'}, {"LOAD", 't'}, {"STORE", 't'}, {"INPUT", 't'}, {"OUTPUT", 't'}, {"STOP", 't'}, 
    // Diretivas
    {"SPACE", 'd'}, {"CONST", 'd'}, {"BEGIN", 'c'}, {"END", 'd'}, {"EXTERN", 'c'}, {"PUBLIC", 'c'}
};

std::map<std::string, int> op_size_map = {
    // Instruções
    {"ADD", 2}, {"SUB", 2}, {"MUL", 2}, {"DIV", 2}, {"JMP", 2}, {"JMPN", 2}, {"JMPP", 2},{"JMPZ", 2},
    {"COPY", 3}, {"LOAD", 2}, {"STORE", 2}, {"INPUT", 2}, {"OUTPUT", 2}, {"STOP", 1}, 
    // Diretivas
    {"SPACE", 1}, {"CONST", 1}, {"BEGIN", 0}, {"END", 0}, {"EXTERN", 0}, {"PUBLIC", 0}
};

std::map<std::string, std::string> op_code_map = {
    // Operações
    {"ADD", "1"}, {"SUB", "2"}, {"MUL", "3"}, {"DIV", "4"}, {"JMP", "5"}, {"JMPN", "6"}, {"JMPP", "7"},{"JMPZ", "8"},
    {"COPY", "9"}, {"LOAD", "10"}, {"STORE", "11"}, {"INPUT", "12"}, {"OUTPUT", "13"}, {"STOP", "14"},
};



