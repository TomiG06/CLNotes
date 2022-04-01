#ifndef NOTES_H
#define NOTES_H

#include "csv.h"

typedef struct note {
    char content[100];
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
            ret.content[x] = 0x0;
            ret.completed = line[x+1]-48;
            return ret;
        }
        ret.content[x] = line[x];
    }
}

note* instances() {
    uint16_t ln = lines();
    if(!ln) return NULL;
    char* content = readDB();
    char* lineBuff = (char*) malloc(105);
    /*
        lineCount helps keep track of lineBuff's next free cell
        whereas noteCount helps keep track of ret's next free cell (ret is declared 4 lines below)
    */
    uint16_t lineCount = 0, noteCount = 0;
    char c;
    note* ret = (note*)malloc(ln *sizeof(note));
    ln = strlen(content); //Was not used again and O(2n) > O(n^2)
    for(size_t x = 0; x<ln; ++x) {
        c = content[x];
        if(c == 0xA) {
            lineCount = 0;
            ret[noteCount++] = extractNote(lineBuff);
            memset(lineBuff, 0, 105);
            continue;
        } else if(c == EOF) break;
        lineBuff[lineCount++] = c;
    }
    free(content);
    free(lineBuff);
    return ret;
}

char addNote(char* content) {
/*
    +2 Instance in db      (fail)
    +1 Successfully added
     0 Longer than allowed (fail)
    -1 Can't add more        ''
*/
    if(strlen(content) > 100) return 0; //check if length is more than allowed
    uint8_t ln = lines();
    if(ln == 100) return -1; //check if lines are 100, which means that limit is reached
    note* all = instances();
    for(uint8_t x = 0; x<ln; ++x) {
        if(!strcmp(all[x].content, content)) { //check if instance already exists
            free(all);
            return 2;
        }
    }
    free(all);
    char instance[105];
    sprintf(instance, "%s%c0\n", content, DEL);
    writeDB(instance, "a");
    addLines(1);
    return 1;
}

uint8_t updateNote(uint16_t line) {
    if(line < 1 || line > lines()) return 0;
    update(line);
    return 1;
}

char deleteNote(uint16_t line) {
    uint8_t ln = lines();
    if(line < 0 || line > ln-1) return 0;
    note* all = instances();
    for(line; line<ln; line++) all[line] = all[line+1]; //move every line, after the one to be deleted, -1 lines
    ln -= 1; //a note is deleted so ln must decrement
    FILE *f = fopen(DB, "w");
    for(uint8_t x = 0; x<ln; ++x) fprintf(f, "%s%c%d\n", all[x].content, DEL, all[x].completed);
    fclose(f);
    free(all);
    addLines(-1);
    return 1;
}

char deleteByStatus(char* status) {
    if(strcmp(status, "-x") && strcmp(status, "-v")) return 0;
    //strcmp returns 0 if args are equal, -v means we want 1, so if it is -v then !0 -> 1. Else, !(int /= 0) -> 0
    char statuss = !strcmp(status, "-v"); 
    note* all = instances();
    size_t tracker = 0;
    size_t ln = lines();
    for(size_t x = 0; x<ln; ++x) {
        if(all[x].completed == statuss) deleteNote(tracker);
        else tracker++;
    }
    free(all);
    return 1;
}

#endif
