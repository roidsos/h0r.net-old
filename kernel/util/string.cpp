#include <util/string.h>

uint_8 strlen(char* str){
    uint_8 i = 0;
    while(str[i] != 0){
    i++;
    }
    return i;
}