#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DB "../db/notes.csv"
#define LINES "../db/lines.txt"

uint16_t lines() {
    uint8_t ln;
    FILE* f = fopen(LINES, "r");
    fscanf(f, "%hhd", &ln);
    fclose(f);
    return ln;
}

void addLines(int8_t num) {
    uint8_t ln = lines() + num;
    FILE *f = fopen(LINES, "w");
    fprintf(f, "%hu", ln);
    fclose(f);
}

char* readDB() {
    FILE *f = fopen(DB, "r");
    char c;
    uint16_t x = 0;
    char* content = (char*) malloc(105*lines());
    while((c = getc(f)) != EOF) content[x++] = c;
    return content;
}

void writeDB(char* content, char* status) {
    FILE *f = fopen(DB, status);
    fprintf(f, "%s", content);
    fclose(f);
}

void update(uint8_t line) {
    char* content = readDB();
    size_t counter = 0;
    for(size_t x = 0; x<strlen(content); x++) {
        if(content[x] == '~') counter++; //0x1F
        if(counter == line) {
            content[x+1] = content[x+1] == 49 ? 48: 49;
            break;
        }
    }
    FILE *f = fopen(DB, "w");
    fprintf(f, "%s", content);
    fclose(f);
    free(content);
}

#endif
