#ifdef HEAP_ACCESSABLE
#include "ini.h"
#include <libk/string.h>
#include <libk/stdtypes.h>
#include <libk/string.h>
#include <libk/vector.h>
#include <utils/heapdef.h>

vector_static(struct key_value_pair, kvps);

struct parsed_ini parse_ini(char *string) {
    vector_init(&kvps);
    char currtag[255];
    uint8_t eqindex = 0;
    char *token = strtok(string, "\n");
    while (token != NULL) {
        if (token[0] == '[') {
            for (size_t j = 1; j < strlen(token) - 1; j++) {
                if (token[j] == ']') {
                    currtag[j - 1] = 0;
                } else {
                    currtag[j - 1] = token[j];
                }
            }
        } else {
            struct key_value_pair kvp;
            char *eqsign = strchr(token, '=');
            if (eqsign != NULL) {
                eqindex = eqsign - token;
                kvp.key = (char *)malloc(strlen(currtag) + eqindex + 2);
                memcpy(kvp.key, currtag, strlen(currtag));
                kvp.key[strlen(currtag)] = '.';
                memcpy(kvp.key + strlen(currtag) + 1, token, eqindex);
                kvp.key[strlen(currtag) + eqindex + 1] = 0;

                kvp.value = (char *)malloc(strlen(token) - eqindex);
                memcpy(kvp.value, eqsign + 1, strlen(token) - eqindex - 1);
                kvp.value[strlen(token) - eqindex] = 0;

                vector_push_back(&kvps, kvp);
            }
        }
        token = strtok(NULL, "\n");
    }
    struct parsed_ini pini;
    pini.count = vector_length(&kvps);
    pini.data = kvps.data;
    return pini;
}

#endif