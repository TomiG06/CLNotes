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

int main(int argc, char* argv[]) {
    if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "-C")) {
        if(argc == 2) {
            printf("Second argument must be the note you want to add\n");
            exit(1);
        }
        char as = addNote(argv[2]);
        if(as == 1) printf("Successfully added!\n");
        else if(!as) printf("Note lengthier than allowed\nLength: %ld\nMaximum: 50\n", strlen(argv[2]));
        else printf("Maximum storage\n");
    } else if(!strcmp(argtolower(argv[1]), "-r")) {
        if(argc == 2 || !strcmp(argv[2], ".")) displayNotes(-1);
        if(!strcmp(argtolower(argv[2]), "-v")) displayNotes(1);
        else if(!strcmp(argtolower(argv[2]), "-x")) displayNotes(0);
        else printf("Uknown status\n");
    } else if(!strcmp(argtolower(argv[1]), "-u")) {
        if(argc == 2) {
            printf("No input note\n");
            exit(1);
        }
        uint16_t ln = lines();
        for(size_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x])) {
                printf("Expected number, got '%s'\n", argv[x]);
                continue;
            }
            uint8_t us = updateNotes(atoi(argv[x]), ln);
            if(us) printf("Note %s updated successfully\n", argv[x]);
            else printf("Number %s out of range\nRange: 0 ... %d\n", argv[2], ln);
        }
    } else if(!strcmp(argtolower(argv[1]), "-d")) {
        if(argc == 2) {
            printf("No input note\n");
            exit(1);
        }
        if(!strcmp(argv[2], ".")) {
            writeDB("", "w");
            addLines(-lines());
            return 0;
        }
        if(!argisdigit(argv[2])) {
            if(!deleteByStatus(argtolower(argv[2]))) printf("Uknown status '%s'\n", argv[2]);
            exit(0);
        }
        for(size_t x = 2; x<argc; ++x) {
            if(!argisdigit(argv[x])) {
                printf("Expected number, got '%s'\n", argv[x]);
                continue;
            }
            char ds = deleteNote(atoi(argv[2])-1);
            if(ds) printf("Note %s deleted successfully\n", argv[x]);
            else printf("Number %s out of range\nRange: 0 ... %d\n", argv[2], lines());
        }
    } else printf("Uknown command '%s'\n", argv[1]);
    return 0;
}
