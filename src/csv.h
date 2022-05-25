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

uint8_t lines();
void addLines(int8_t num);
void writeDB(char* content, char* status);

#endif
