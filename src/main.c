#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "notes.h"
#include "csv.h"
#include "helpers.h"

int main(int argc, char* argv[]) {
    #ifdef DEBUG
        printf("\nDEBUGGING\n\n");
    #endif

    for(size_t x = 1; x<argc; ++x) { //Loop through every argument in order to not have to check for both uppper and lower case
        argtolower(argv[x]);
        if(!strcmp(argv[x], "-c")) break; //Next argument is message but we want it as is
    }

    if(!strcmp(argv[1], "-c")) {
        if(argc != 3) {
            fprintf(stderr, "Must receive 3 arguments\nReceived %d\n", argc);
            return 1;
        }
        char as = addNote(argv[2]);
        if(as != 1) {
            if(!as) fprintf(stderr, "Note lengthier than allowed\nLength: %ld\nMaximum: %d\n", strlen(argv[2]), MAX_LENGTH);
            else if(as == 2) fprintf(stderr, "Instance already exists\n");
            else fprintf(stderr, "You have reached the maximum amount of notes allowed\n"); //Might delete the max notes but who will ever write 100 notes at once?
            return 1;
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
