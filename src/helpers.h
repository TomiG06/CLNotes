#ifndef HELPERS_H
#define HELPERS_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "notes.h"

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
    note* arr = read_instances();
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


    printf("Args:\n");
    printf("\t-x/-v: executes command on unchecked/checked notes\n\t\tUsed in: -r, -d\n");
    printf("\t-a: executes command on every note\n\t\tUsed in: -r, -d");
    printf("\n\tOn -d and -u commands, numbers can be used in order to execute the command on the coresponding notes\n\n");


    printf("Commands: (no need to worry about case sensitivity)\n");
    printf("\t-h: help\n");
    printf("\t\tArg: None\n");
    printf("\t-c: create instance\n");
    printf("\t\tArg: string to store as note\n");
    printf("\t-r: read instances\n");
    printf("\t\tArg: -x, -v, -a\n");
    printf("\t-u: update instances' status\n");
    printf("\t\tArg: numbers\n");
    printf("\t-d: delete instances\n");
    printf("\t\tArg: -x, -v, -a, numbers\n");
}

#endif
