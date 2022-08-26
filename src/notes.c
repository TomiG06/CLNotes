#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "csv.h"
#include "notes.h"

note extractNote(char* record) {
    note ret;

    puts(record);

    for(uint16_t x = 0; x < strlen(record); x++) {
        if(record[x] == DEL) {
        /*
            If delimeter is met:
                Content gets NULL terminated
                Next char is status => (
                    1 -> Completed
                    0 -> !1
                )
            else the character is pushed in the content because it belongs there
        */
            ret.content[x++] = 0x0;
            ret.completed = record[x]-48;
            break;
        }
        ret.content[x] = record[x];
    }

    return ret;
}

note* read_instances() {
    /*
        reads the file where we store our notes
        and returns an array of note type
    */

    /*
        store len in var in order not to have
        to do the addition every time
    */
    size_t len = MAX_LENGTH+3;
    char buffer[MAX_LENGTH+3];
    uint16_t notesIndex = 0;
    note* ret = (note*)malloc(records() * sizeof(note));

    FILE* f = fopen(DB, "r");

    while(read_record(buffer, f)) {
        ret[notesIndex++] = extractNote(buffer);
        memset(buffer, 0, len);
    }

    fclose(f);

    return ret;
}

void write_instances(note* instances) {
    /*
        takes an array of note type
        and writes the content on DB
    */
    uint8_t rec = records();
    FILE* f = fopen(DB, "w");
    for(size_t x = 0; x < rec; x++) {
        fprintf(f, "%s%c%d%c", instances[x].content, DEL, instances[x].completed, REC_SEP);
    }
    fclose(f);
}

char addNote(char* content) {
/*
    +2 Instance in db      (fail)
    +1 Successfully added
     0 Longer than allowed (fail)
    -1 Can't add more        ''
*/
    if(strlen(content) > MAX_LENGTH) return 0; //check if length is more than allowed
    note* all = read_instances();
    for(uint16_t x = 0; x < records(); ++x) {
        if(!strcmp(all[x].content, content)) { //check if instance already exists
            free(all);
            return 2;
        }
    }
    free(all);

    char instance[MAX_LENGTH+3];
    sprintf(instance, "%s%c0%c", content, DEL, REC_SEP); //setting up instance
    writeDB(instance, "a"); //writing it to DB
    add_records(1); //increment record count
    return 1;
}

uint8_t updateNote(int16_t record, note* instances) {
    if(record < 1 || record > records()) return 0;
    instances[record-1].completed = !instances[record-1].completed;
    return 1;
}

char deleteNote(int16_t record, note* notes) {
    uint8_t recs = records();

    if(record < 0 || record > recs-1) return 0; //Check if line is valid

    while(record++ < recs) {
        notes[record-1] = notes[record]; //move every line, after the one to be deleted, -1 lines
    }

    add_records(-1);
    return 1;
}

char deleteByStatus(char* status) {
    if(strcmp(status, "-x") && strcmp(status, "-v")) return 0;
    //strcmp returns 0 if args are equal, -v means we want 1, so if it is -v then !0 -> 1. Else, !(int /= 0) -> 0
    char numeric_status = !strcmp(status, "-v");
    note* notes = read_instances();
    
    for(int16_t x = records() -1; x > -1; x--) {
        if(notes[x].completed == numeric_status) deleteNote(x, notes);
    }

    write_instances(notes);
    free(notes);
    return 1;
}
