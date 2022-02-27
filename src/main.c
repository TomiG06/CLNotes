#include "notes.h"

void displayNotes(int status, note* arr) {
    if(status < -1 || status > 1) printf("Uknown status\n");
    for(size_t x = 0; x<lines(); x++) 
        if(status == -1 || status == arr[x].checked) 
            printf("%c %lu. '%s'\n", arr[x].checked ? 'v': 'x', x+1, arr[x].content);
}


/*

    -----Commands-----
    (Not case sensitive)
    '-c': create a note
    '-r': read notes
    '-u': update status
    '-d': delete note

*/

int main(int argc, char* argv[]) {
    if(argc > 3) {
        printf("CLNotes expects no more than 2 args\n");
        exit(1);
    }
    if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "-C")) {
        if(argc == 2) {
            printf("Second argument must be the note you want to add\n");
            exit(1);
        }
        char as = addNote(argv[2]);
        if(as == 1) printf("Successfully added!\n");
        else if(!as) printf("Note lengthier than allowed\nLength: %ld\nMaximum: 50\n", strlen(argv[2]));
        else printf("Maximum storage\n");
    } else if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "-R")) {
        note* allNotes = instances();
        if(argc == 2 || !strcmp(argv[2], ".")) displayNotes(-1, allNotes);
        else displayNotes(atoi(argv[2]), allNotes);
        free(allNotes);
    } else if(!strcmp(argv[1], "-u") || !strcmp(argv[1], "-U")) {
        uint8_t us = updateNote(atoi(argv[2]));
        if(us) printf("Note %s updated successfully\n", argv[2]);
        else printf("Number %s out of range\nRange: 0 ... %d\n", argv[2], lines());
    } else if(!strcmp(argv[1], "-d") || !strcmp(argv[1], "-D")) {
        char ds = deleteNote(atoi(argv[2])-1);
        if(ds) printf("Note %s deleted successfully\n", argv[2]);
        else printf("Number %s out of range\nRange: 0 ... %d\n", argv[2], lines());
    } else printf("Uknown command '%s'\n", argv[1]);
    return 0;
}
