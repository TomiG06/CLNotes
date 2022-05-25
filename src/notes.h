#ifndef NOTES_H
#define NOTES_H

#include <stdint.h>
#include "csv.h"

typedef struct note {
    char content[MAX_LENGTH];
    uint8_t completed; //For todos
} note;

note extractNote(char* line);
note* read_instances();
void write_instances(note* instances);
char addNote(char* content);
uint8_t updateNote(uint16_t line, note* instances);
char deleteNote(uint16_t line, note* notes);
char deleteByStatus(char* status);

#endif
