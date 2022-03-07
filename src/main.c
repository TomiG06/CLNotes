#include "helpers.h"

int main(int argc, char* argv[]) {
    char* cmd = argtolower(argv[1]); //unfreed memory
    if(!strcmp(cmd, "-c")) {
        if(argc == 2) {
            fprintf(stderr, "Second argument must be the note you want to add\n");
            return 1;
        }
        char as = addNote(argv[2]);
        if(as == 1) printf("Successfully added!\n");
        else {
            if(!as) fprintf(stderr, "Note lengthier than allowed\nLength: %ld\nMaximum: 100\n", strlen(argv[2]));
            else if(as == -2) fprintf(stderr, "Note can't contain delimeter '~'\n");
            else fprintf(stderr, "You have reached the maximum amount of notes allowed\n");
            return 1;
        }
    } else if(!strcmp(cmd, "-r")) {
        if(argc == 2 || !strcmp(argv[2], ".")) displayNotes(-1);
        if(!strcmp(argtolower(argv[2]), "-v")) displayNotes(1);
        else if(!strcmp(argtolower(argv[2]), "-x")) displayNotes(0);
        else {
            fprintf(stderr, "Uknown status\n");
            return 1;
        }
    } else if(!strcmp(cmd, "-u")) {
        if(argc == 2) {
            fprintf(stderr, "No input note\n");
            return 1;
        }
        uint8_t ln = lines();
        for(uint8_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x])) {
                fprintf(stderr, "Expected number, got '%s'\n", argv[x]);
                continue;
            }
            uint8_t us = updateNote(atoi(argv[x]), ln);
            if(us) printf("Note %s updated successfully\n", argv[x]);
            else fprintf(stderr, "Number %s out of range\nRange: 0 ... %d\n", argv[2], ln);
        }
    } else if(!strcmp(cmd, "-d")) {
        if(argc == 2) {
            fprintf(stderr, "No input note\n");
            return 1;
        }
        if(!strcmp(argv[2], ".")) {
            writeDB("", "w");
            addLines(-lines());
            return 0;
        }
        if(!argisdigit(argv[2])) {
            if(!deleteByStatus(argtolower(argv[2]))) {
                fprintf(stderr, "Uknown status '%s'\n", argv[2]);
                return 1;
            }
            return 0;
        }
        for(size_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x])) {
                fprintf(stderr, "Expected number, got '%s'\n", argv[x]);
                continue;
            }
            char ds = deleteNote(atoi(argv[2])-1);
            if(ds) printf("Note %s deleted successfully\n", argv[x]);
            else fprintf(stderr, "Number %s out of range\nRange: 0 ... %d\n", argv[2], lines());
        }
    } else if(!strcmp(cmd, "-h")) {
        help();
    } else {
        fprintf(stderr, "Uknown command '%s'\n", argv[1]);
        return 1;
    }
    return 0;
}
