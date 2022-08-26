#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "notes.h"
#include "csv.h"

int* bsort(char* argv[], size_t len) {
    //Bubble sort algorithm
    //O(n^2)
    int* sorted = (int*) malloc(sizeof(int) * (len));
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

    for(size_t x = 0; x<records(); x++) 
        if(status == -1 || status == arr[x].completed) 
            printf("%c %lu. %s\n", arr[x].completed ? 'v': 'x', x+1, arr[x].content);

    free(arr);
}

char argisdigit(char* value) {
    for(size_t x = 0; x<strlen(value); x++) if(!isdigit(value[x])) return 0;
    return 1;
}

int main(int argc, char* argv[]) {
    #ifdef DEBUG
        printf("\nDEBUGGING\n\n");
    #endif


    if(!strcmp(argv[1], "-c")) {
        char add_status;
        for(int x = 2; x < argc; x++) {
            if((add_status = addNote(argv[x])) != 1) {
                if(!add_status) fprintf(stderr, "Note lengthier than allowed\nLength: %ld\nMaximum: %d\n", strlen(argv[2]), MAX_LENGTH);
                else if(add_status == 2) fprintf(stderr, "Instance already exists\n");
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
        for(uint16_t x = 2; x<argc; ++x) {
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
            add_records(-records());
            return 0;
        }

        if(!argisdigit(argv[2])) {
            if(!deleteByStatus(argv[2])) {
                fprintf(stderr, "Uknown status '%s'\n", argv[2]);
                return 1;
            }
            return 0;
        }
        
        for(int i = 2; i < argc; ++i) {
            if(!argisdigit(argv[i])) {
                fprintf(stderr, "Argument '%s' is not a number\n", argv[i]);
                return 1;
            }
        }

        note* notes = read_instances();
        size_t len = argc-2;
        int* sorted = bsort(argv, len); //Must be sorted in order to be certain that we are deleting the right stuff

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
    } else {
        fprintf(stderr, "Uknown command '%s'\nTry 'clnotes -h' for more info\n", argv[1]);
        return 1;
    }
    return 0;
}
