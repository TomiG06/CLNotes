#ifndef HELPERS_H
#define HELPERS_H

#include "notes.h"
#include <ctype.h>

void displayNotes(char status) {
    if(status < -1 || status > 1) printf("Uknown status\n");
    note* arr = instances();
    for(size_t x = 0; x<lines(); x++) if(status == -1 || status == arr[x].checked) printf("%c %lu. '%s'\n", arr[x].checked ? 'v': 'x', x+1, arr[x].content);
    free(arr);
    exit(0);
}

char argisdigit(char* value) {
    for(size_t x = 0; x<strlen(value); x++) if(!isdigit(value[x])) return 0;
    return 1;
}

char* argtolower(char* value) {
    size_t ln = strlen(value);
    char* ret = (char*) malloc(ln);
    for(size_t x = 0; x<ln; x++) ret[x] = tolower(value[x]);
    return ret;
}

#endif