#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DB "../db/notes.csv"
#define LINES "../db/lines.txt"

uint16_t lines() {
    uint16_t ln;
    FILE* f = fopen(LINES, "r");
    fscanf(f, "%hd", &ln);
    fclose(f);
    return ln;
}

void addLines(int16_t num) {
    uint16_t ln = lines() + num;
    FILE *f = fopen(LINES, "w");
    fprintf(f, "%hu", ln);
    fclose(f);
}

char* readDB() {
    FILE *f = fopen(DB, "r");
    char c;
    uint16_t x = 0;
    char* content = (char*) malloc(60*lines());
    while((c = getc(f)) != EOF) content[x++] = c;
    return content;
}

void append(char* content) {
    FILE *f = fopen(DB, "a");
    fprintf(f, "%s", content);
    fclose(f);
}

void update(uint16_t line) {
    char* content = readDB();
    size_t counter = 0;
    for(size_t x = 0; x<strlen(content); x++) {
        if(content[x] == '~') counter++;
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
