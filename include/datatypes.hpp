#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

struct fileData {
    std::string name;
    std::string content;
};

struct compilationError {
    std::string type;
    std::string message;
    int line;
};

struct tokenMatrix {
    int lines;
    std::vector<std::vector<std::string>> matrix;
};

struct dependeciesData{
    int address;
    int space_arg = 0;
};

struct symbolData {
    std:: string name;
    int value;
    bool is_valueRelative;
    bool is_defined;
    bool is_extern;
    std::vector<dependeciesData> list_of_dependencies;
    int line = 0;
    std::string section;
};

struct outputObj {
    std::vector<symbolData> use_table;
    std::vector<symbolData> definition_table;
    std::vector<std::string> relative_table;
    std::vector<std::string> assembled_code;
};

#endif