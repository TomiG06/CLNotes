#ifndef HELPERS_H
#define HELPERS_H

#include "notes.h"
#include <ctype.h>

char argisdigit(char* value);
void argtolower(char* arg);

int* bsort(char* argv[], int argc) {
    //Bubble sort algorithm
    //O(n^2)
    int* sorted = (int*) malloc(sizeof(int) * (argc-2));
    int len = argc-2;
    int temp;

    for(size_t i = 0; i < len; ++i) sorted[i] = atoi(argv[i+2]);

    for(size_t i = 0; i < len; ++i) {
        for(size_t j = i+1; j < len; ++j) {
            if(sorted[i] < sorted[j]) {
                temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    return sorted;
}

void displayNotes(char status) {
    note* arr = instances();
    for(size_t x = 0; x<lines(); x++) if(status == -1 || status == arr[x].completed) printf("%c %lu. %s\n", arr[x].completed ? 'v': 'x', x+1, arr[x].content);
    free(arr);
}

char argisdigit(char* value) {
    for(size_t x = 0; x<strlen(value); x++) if(!isdigit(value[x])) return 0;
    return 1;
}

void argtolower(char* arg) {
    for(size_t x = 0; x<strlen(arg); x++) *(arg + x) = tolower(arg[x]);
}

void help() {
    printf("usage: clnotes [command] [args]\n\n");
    printf("Status args: '-v', '-x'\n");
    printf("All args: '-a', ''(-> on read)\n");
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
