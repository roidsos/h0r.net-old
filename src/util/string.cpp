#include <util/string.h>
#include <drivers/memory/Heap.h>
// ok, lets use this bitch yup
uint_8 strlen(char* str){
    uint_8 i = 0;
    while(str[i] != 0){
    i++;
    }
    return i;
}

bool strcmp(char* str1,char* str2)
{
    int i = 0;
    if(strlen(str1) != strlen(str2))
        return false;
    while (str1[i] != 0 && str2[i] != 0)
    {
        if (str1[i] != str2[i]){
            return false;
        }
        i++;
    }
    return true;    
}

char** split(char* str, char spliton) {
    int num_substrings = 1;
    for (int i = 0; str[i]; i++) {
        if (str[i] == spliton) {
            num_substrings++;
        }
    }

    char** substrings = (char**) malloc(((num_substrings + 1) * sizeof(char*)));
    int substr_index = 0;
    int start_index = 0;

    for (int i = 0; str[i]; i++) {
        if (str[i] == spliton) {
            int substring_length = i - start_index;
            substrings[substr_index] = (char*) malloc((substring_length + 1));
            for (int j = 0; j < substring_length; j++) {
                substrings[substr_index][j] = str[start_index + j];
            }
            substrings[substr_index][substring_length] = '\0';
            start_index = i + 1;
            substr_index++;
        }
    }

    int substring_length = strlen(str) - start_index;
    substrings[substr_index] = (char*) malloc((substring_length + 1 ));
    for (int j = 0; j < substring_length; j++) {
        substrings[substr_index][j] = str[start_index + j];
    }
    substrings[substr_index][substring_length] = '\0';
    substr_index++;

    substrings[substr_index] = nullptr;

    return substrings;
}
