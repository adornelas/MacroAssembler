#include "../include/utils.hpp"

bool isLabel(std::string str){
    for( int i = 0 ; i < str.size() ; i++){
        if(str[i] == ':'){
            return true;
        }
    }

    return false;
}