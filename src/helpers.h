#ifndef HELPERS_H
#define HELPERS_H

#include "notes.h"
#include <ctype.h>

void displayNotes(char status) {
    note* arr = instances();
    for(size_t x = 0; x<lines(); x++) if(status == -1 || status == arr[x].completed) printf("%c %lu. '%s'\n", arr[x].completed ? 'v': 'x', x+1, arr[x].content);
    free(arr);
}

char argisdigit(char* value) {
    for(size_t x = 0; x<strlen(value); x++) if(!isdigit(value[x])) return 0;
    return 1;
}

char* argtolower(char value[]) {
    for(size_t x = 0; x<strlen(value); x++) value[x] = tolower(value[x]);
    return value;
}

void help() {
    printf("usage: clnotes [command] [args]\n");
    printf("\nStatus args: '-v', '-x'\n");
    printf("All args: '.', '' (Note: blank doesn't work on '-d')\n");
    printf("Nums args: Number\\s corresponding to note\\s (you can check them with 'clnotes -r')\n\n");
    printf("Commands: (no need to worry about case sensitivity)\n");
    printf("\t-h: help\n");
    printf("\t\tArg: None\n");
    printf("\t-c: create instance\n");
    printf("\t\tArg: instance to store (string)\n");
    printf("\t-r: read instances\n");
    printf("\t\tArg: status or all\n");
    printf("\t-u: update instances' status\n");
    printf("\t\tArg\\s: nums to update\n");
    printf("\t-d: delete instances\n");
    printf("\t\tArg\\s: status, all or nums\n");
}

#endif
