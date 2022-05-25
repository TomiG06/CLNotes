#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "notes.h"
#include "csv.h"

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

int main(int argc, char* argv[]) {
    #ifdef DEBUG
        printf("\nDEBUGGING\n\n");
    #endif

    for(size_t x = 1; x<argc; ++x) { //Loop through every argument in order to not have to check for both uppper and lower case
        argtolower(argv[x]);
        if(!strcmp(argv[x], "-c")) break; //Next argument is message but we want it as is
    }

    if(!strcmp(argv[1], "-c")) {
        char add_status;
        for(int x = 2; x < argc; x++) {
            if((add_status = addNote(argv[x])) != 1) {
                if(!add_status) fprintf(stderr, "Note lengthier than allowed\nLength: %ld\nMaximum: %d\n", strlen(argv[2]), MAX_LENGTH);
                else if(add_status == 2) fprintf(stderr, "Instance already exists\n");
                else fprintf(stderr, "You have reached the maximum amount of notes allowed\n"); //Might delete the max notes but who will ever write 100 notes at once?
                return 1;
            }
        }
    } else if(!strcmp(argv[1], "-r")) {
        if(argc == 2 || !strcmp(argv[2], "-a")) displayNotes(-1);
        else if(!strcmp(argv[2], "-v")) displayNotes(1);
        else if(!strcmp(argv[2], "-x")) displayNotes(0);
        else {
            fprintf(stderr, "Uknown status '%s'\n", argv[2]);
            return 1;
        }
    } else if(!strcmp(argv[1], "-u")) {
        if(argc == 2) {
            fprintf(stderr, "No input note\n");
            return 1;
        }
        note* notes = read_instances();
        for(uint8_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x]) || !updateNote(atoi(argv[x]), notes)) {
                write_instances(notes);
                free(notes);
                fprintf(stderr, "Illegal number '%s'\n", argv[x]);
                return 1;
            }
        }
        write_instances(notes);
        free(notes);
    } else if(!strcmp(argv[1], "-d")) {
        if(argc == 2) {
            fprintf(stderr, "No input note\n");
            return 1;
        }
        if(!strcmp(argv[2], "-a")) {
            writeDB("", "w");
            addLines(-lines());
            return 0;
        }

        if(!argisdigit(argv[2])) {
            if(!deleteByStatus(argv[2])) {
                fprintf(stderr, "Uknown status '%s'\n", argv[2]);
                return 1;
            }
            return 0;
        }
        
        for(size_t i = 2; i < argc; ++i) {
            if(!argisdigit(argv[i])) {
                fprintf(stderr, "Argument '%s' is not a number\n", argv[i]);
                return 1;
            }
        }

        note* notes = read_instances();
        int* sorted = bsort(argv, argc); //Must be sorted in order to be certain that we are deleting the right stuff
        int len = argc-2;

        for(size_t x = 0; x<len; ++x) {
            if(!deleteNote(sorted[x]-1, notes)) {
                write_instances(notes);
                free(notes);
                fprintf(stderr, "Illegal number '%d'\n", sorted[x]);
                return 1;
            }
        }
        write_instances(notes);
        free(notes);
    } else if(!strcmp(argv[1], "-h")) {
        help();
    } else {
        fprintf(stderr, "Uknown command '%s'\nTry 'clnotes -h' for more info\n", argv[1]);
        return 1;
    }
    return 0;
}
