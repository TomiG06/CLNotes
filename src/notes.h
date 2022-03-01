#ifndef NOTES_H
#define NOTES_H

#include "csv.h"

typedef struct noteStruct {
    char content[60];
    uint8_t checked;
} note;

note extractNote(char* line) {
    uint8_t count = 0;
    uint8_t x = 0;
    note ret;
    for(x; x < strlen(line); x++) {
        if(line[x] == '~') {
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
        printf("Database is empty\nAdd stuff with 'clnotes -c \"your note/todo\"'\n");
        exit(0);
    }
    char* content = readDB();
    char* lineBuff = (char*) malloc(60);
    uint16_t lineCount = 0;
    uint16_t noteCount = 0;
    char c;
    note resm;
    note* ret = (note*)malloc(ln *sizeof(note));
    for(size_t x = 0; x<strlen(content); ++x) {
        c = content[x];
        if(c == 0xA) {
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
    if(lines() == 100) return -1;
    char instance[60];
    sprintf(instance, "%s~%d\n", content, 0);
    writeDB(instance, "a");
    addLines(1);
    return 1;
}

uint8_t updateNotes(uint16_t line, uint16_t ln) {
    if(line < 1 || line > ln) return 0;
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
    for(uint16_t x = 0; x<ln; ++x) fprintf(f, "%s~%d\n", all[x].content, all[x].checked);
    fclose(f);
    free(all);
    addLines(-1);
    return 1;
}

char deleteByStatus(char* status) {
    if(strcmp(status, "-x") && strcmp(status, "-v")) return 0;
    char statuss = strcmp(status, "-x")? 1: 0;
    note* all = instances();
    size_t tracker = 0;
    size_t ln = lines();
    for(size_t x = 0; x<ln; ++x)
        if(all[x].checked == statuss) deleteNote(tracker);
        else tracker++;
    return 1;
}

#endif
