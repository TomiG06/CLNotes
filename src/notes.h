#ifndef NOTES_H
#define NOTES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

typedef struct note {
    char content[MAX_LENGTH];
    uint8_t completed; //For todos
} note;

note extractNote(char* line) {
    note ret;
    for(uint8_t x = 0; x < strlen(line); x++) {
        if(line[x] == DEL) {
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
            ret.completed = line[x]-48;
            return ret;
        }
        ret.content[x] = line[x];
    }
}

note* read_instances() {
    /*
        reads the file where we store our notes
        and returns an array of note type
    */
    uint16_t ln = lines();

    //returns null if there are no instances stored
    if(!ln) return NULL;

    /*
        store len in var in order not to have
        to do the addition every time
    */
    size_t len = MAX_LENGTH+3;
    char* buffer = (char*) malloc(len);
    uint16_t notesIndex = 0;
    note* ret = (note*)malloc(ln * sizeof(note));

    FILE* f = fopen(DB, "r");

    while(fgets(buffer, len, f)) {
        buffer[strlen(buffer)-1] = 0;
        ret[notesIndex++] = extractNote(buffer);
        memset(buffer, 0, len);
    }

    fclose(f);
    free(buffer);
    return ret;
}

void write_instances(note* instances) {
    /*
        takes an array of note type
        and writes the content on DB
    */
    uint8_t ln = lines();
    FILE* f = fopen(DB, "w");
    for(size_t x = 0; x < ln; x++) {
        fprintf(f, "%s%c%d\n", instances[x].content, DEL, instances[x].completed);
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
    uint8_t ln = lines();
    if(ln == 100) return -1; //check if lines are 100, which means that limit is reached
    note* all = read_instances();
    for(uint8_t x = 0; x<ln; ++x) {
        if(!strcmp(all[x].content, content)) { //check if instance already exists
            free(all);
            return 2;
        }
    }
    free(all);

    char instance[MAX_LENGTH+3];
    sprintf(instance, "%s%c0\n", content, DEL); //setting up instance
    writeDB(instance, "a"); //writing it to DB
    addLines(1); //increment lines
    return 1;
}

uint8_t updateNote(uint16_t line, note* instances) {
    if(line < 1 || line > lines()) return 0;
    instances[line-1].completed = !instances[line-1].completed;
    return 1;
}

char deleteNote(uint16_t line, note* notes) {
    uint8_t ln = lines();

    if(line < 0 || line > ln-1) return 0; //Check if line is valid

    for(line; line<ln; line++) {
        notes[line] = notes[line+1]; //move every line, after the one to be deleted, -1 lines
    }

    addLines(-1);
    return 1;
}

char deleteByStatus(char* status) {
    if(strcmp(status, "-x") && strcmp(status, "-v")) return 0;
    //strcmp returns 0 if args are equal, -v means we want 1, so if it is -v then !0 -> 1. Else, !(int /= 0) -> 0
    char numeric_status = !strcmp(status, "-v");
    note* notes = read_instances();
    
    for(int16_t x = lines() -1; x > -1; x--) {
        if(notes[x].completed == numeric_status) deleteNote(x, notes);
    }

    write_instances(notes);
    free(notes);
    return 1;
}

#endif
