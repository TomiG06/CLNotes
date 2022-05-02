#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef DEBUG
    #define DB "testDB.csv"
    #define LINES "testLines.txt"
#else
    #define LINES "../db/lines.txt"
    #define DB "../db/notes.csv"
#endif

#define MAX_LENGTH 100
#define DEL 0x1F

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
    //we use it in order to modify lines when we add/delete a note/todo
    uint8_t ln = lines() + num;
    FILE *f = fopen(LINES, "w");
    fprintf(f, "%hu", ln);
    fclose(f);
}

char* readDB() {
    FILE *f = fopen(DB, "r");
    char c;
    uint16_t x = 0;
    char* content = (char*) malloc((MAX_LENGTH+3)*lines()); 
    /*
        maximum length -> 100 (MAX_LENGTH)
        delimiter      -> 001
        status         -> 001
        new line       -> 001
                          103 bytes to be allocated
    */
    while((c = getc(f)) != EOF) content[x++] = c;
    return content;
}

void writeDB(char* content, char* status) {
    FILE *f = fopen(DB, status);
    //"a" for new | "w" for update/delete
    fprintf(f, "%s", content);
    fclose(f);
}

#endif
