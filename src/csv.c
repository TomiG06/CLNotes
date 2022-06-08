#include <stdio.h>
#include <stdint.h>
#include "csv.h"

uint8_t lines() { //provided that max number of notes is 100, unsigned byte var is good enough
    /*
        decided to store the number of lines in a file
        so that we don't have to loop through the whole content
        and check for every new line character
    */
    uint8_t ln;
    FILE* f = fopen(LINES, "r");
    fscanf(f, "%hhd", &ln);
    fclose(f);
    return ln;
}

void addLines(int8_t num) {
    /*
        we use it in order to modify lines when we add/delete a note/todo
        
        Note: num argument must be signed because the function is also used
        to decrement the number of lines when we delete instances
    */
    uint8_t ln = lines() + num;
    FILE *f = fopen(LINES, "w");
    fprintf(f, "%hu", ln);
    fclose(f);
}

void writeDB(char* content, char* status) {
    FILE *f = fopen(DB, status);
    //"a" for new | "w" for update/delete
    fputs(content, f);
    fclose(f);
}
