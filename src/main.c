#include "helpers.h"

int main(int argc, char* argv[]) {
    for(size_t x = 1; x<argc; ++x) argv[x] = argtolower(argv[x]);
    if(!strcmp(argv[1], "-c")) {
        if(argc != 3) {
            fprintf(stderr, "Must receive 3 arguments\nReceived %d\n", argc);
            return 1;
        }
        char as = addNote(argv[2]);
        if(as != 1) {
            if(!as) fprintf(stderr, "Note lengthier than allowed\nLength: %ld\nMaximum: 100\n", strlen(argv[2]));
            else if(as == -2) fprintf(stderr, "Note can't contain delimeter '~'\n");
            else if(as == 2) fprintf(stderr, "Instance already exists\n");
            else fprintf(stderr, "You have reached the maximum amount of notes allowed\n");
            return 1;
        }
    } else if(!strcmp(argv[1], "-r")) {
        if(argc == 2 || !strcmp(argv[2], ".")) displayNotes(-1);
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
        for(uint8_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x]) || !updateNote(atoi(argv[x]))) {
                fprintf(stderr, "Illegal number '%s'\n", argv[x]);
                return 1;
            }
        }
    } else if(!strcmp(argv[1], "-d")) {
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
            if(!deleteByStatus(argv[2])) {
                fprintf(stderr, "Uknown status '%s'\n", argv[2]);
                return 1;
            }
            return 0;
        }
        for(size_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x]) || !deleteNote(atoi(argv[2])-1)) {
                fprintf(stderr, "Illegal number '%s'\n", argv[x]);
                return 1;
            }
        }
    } else if(!strcmp(argv[1], "-h")) {
        help();
    } else {
        fprintf(stderr, "Uknown command '%s'\n", argv[1]);
        return 1;
    }
    return 0;
}
