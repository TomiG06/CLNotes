#ifndef CSV_H
#define CSV_H

#include <stdint.h>

#ifdef DEBUG
    #define DB      "testDB.csv"
    #define RECORDS "testLines.txt"
#else
    #define DB      "../db/notes.csv"
    #define RECORDS "../db/lines.txt"
#endif

#define MAX_LENGTH  1024
#define DEL         0x1F
#define REC_SEP     0x1E

uint16_t records();
void add_records(int16_t num);
void writeDB(char* content, char* status);
char read_record(char* buffer, FILE* f);

#endif
