#include <stdio.h>
#include <stdint.h>
#include "csv.h"

uint16_t records() {
    /*
        decided to store the number of records in a file
        so that we don't have to loop through the whole content
        and count every REC_SEP
    */
    uint16_t recs;
    FILE* f = fopen(RECORDS, "r");
    fscanf(f, "%hu", &recs);
    fclose(f);
    return recs;
}

void add_records(int16_t num) {
    /* we use it in order to modify the record coumt when we add/delete a note/todo */

    /*
        the new number of records must already be stored in a variable
        before we open the record count file
    */
    uint16_t recs = records() + num;
    FILE *f = fopen(RECORDS, "w");
    fprintf(f, "%hu", recs);
    fclose(f);
}

void writeDB(char* content, char* status) {
    FILE *f = fopen(DB, status);
    fputs(content, f);
    fclose(f);
}

char read_record(char* buffer, FILE* f) {
    char c;
    size_t idx = 0;

    if((c = fgetc(f)) == EOF) return 0;

    do {
        buffer[idx++] = c;
    } while((c = fgetc(f)) != REC_SEP);

    return idx+1;
}

