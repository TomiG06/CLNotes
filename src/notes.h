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
        if(line[x] == '~') {
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
    uint16_t lineCount = 0, noteCount = 0;
    char c;
    note* ret = (note*)malloc(ln *sizeof(note));
    for(size_t x = 0; x<strlen(content); ++x) {
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
    -2 Cointains delimeter   ''
*/
    if(strlen(content) > 100) return 0;
    uint8_t ln = lines();
    if(ln == 100) return -1;
    note* all = instances();
    for(uint8_t x = 0; x<ln; ++x) {
        if(!strcmp(all[x].content, content)) {
            free(all);
            return 2;
        }
    }
    free(all);
    ln = strlen(content); //Was not used again and O(2n) > O(n^2)
    for(uint8_t x = 0; x<ln; ++x) if(content[x] == '~') return -2;
    char instance[105];
    sprintf(instance, "%s~0\n", content);
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
    for(line; line<ln; line++) all[line] = all[line+1];
    --ln;
    FILE *f = fopen(DB, "w");
    for(uint8_t x = 0; x<ln; ++x) fprintf(f, "%s~%d\n", all[x].content, all[x].completed);
    fclose(f);
    free(all);
    addLines(-1);
    return 1;
}

char deleteByStatus(char* status) {
    if(strcmp(status, "-x") && strcmp(status, "-v")) return 0;
    char statuss = !strcmp(status, "-v"); 
    //strcmp returns 0 if args are equal, -v means we want 1, so if it is -v then !0 -> 1
    note* all = instances();
    size_t tracker = 0;
    size_t ln = lines();
    for(size_t x = 0; x<ln; ++x)
        if(all[x].completed == statuss) deleteNote(tracker);
        else tracker++;
    free(all);
    return 1;
}

#endif
