#ifndef NOTES_H
#define NOTES_H

#include "csv.h"

typedef struct noteStruct {
    char content[60];
    uint8_t checked;
} note;

char checkInstance(note inst, char status) {
    if(status == -1) return 1;
    return (status == inst.checked);
}

note extractNote(char* line) {
    short len = strlen(line);
    uint8_t count = 0;
    uint8_t x = 0;
    note ret;
    for(x; x < len; x++) {
        if(line[x] == ',') {
            ret.content[x] = 0x0;
            break;
        }
        ret.content[x] = line[x];
    }
    ret.checked = line[x+1]-48;
    return ret;
}

note* instances() {
    uint16_t ln = lines();
    if(ln == 0) {
        printf("Database is empty\nAdd stuff with 'clnotes -c \"your message\"'\n");
        exit(0);
    }
    char* content = readDB();
    char* lineBuff = (char*) malloc(60);
    uint16_t lineCount = 0;
    uint16_t noteCount = 0;
    uint16_t x = 0;
    char c;
    note resm;
    note* ret = (note*)malloc(ln *sizeof(note));
    for(x; x<strlen(content); ++x) {
        c = content[x];
        if(c == '\n') {
            lineCount = 0;
            resm = extractNote(lineBuff);
            ret[noteCount] = resm;
            noteCount++;
            memset(lineBuff, 0, 65);
            continue;
        } else if(c == EOF) break;
        lineBuff[lineCount] = c;
        lineCount++;
    }
    free(content);
    free(lineBuff);
    return ret;
}

char addNote(char* content) {
/*
    +1 Successfully added
     0 Longer than allowed (fail)
    -1 Can't add more        ''
*/
    if(strlen(content) > 50) return 0;
    if(lines() == 999) return -1;
    char instance[60];
    sprintf(instance, "%s,%d\n", content, 0);
    append(instance);
    addLines(1);
    return 1;
}

uint8_t updateNote(uint16_t line) {
    if(line < 1 || line > lines()) return 0;
    update(line);
    return 1;
}

char deleteNote(uint16_t line) {
    uint16_t ln = lines();
    if(line < 0 || line > ln-1) return 0;
    note* all = instances();
    for(line; line<ln; line++) all[line] = all[line+1];
    --ln;
    FILE *f = fopen(DB, "w");
    for(uint16_t x = 0; x<ln; ++x) fprintf(f, "%s,%d\n", all[x].content, all[x].checked);
    fclose(f);
    free(all);
    addLines(-1);
    return 1;
}

#endif
