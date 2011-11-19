#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "coder.c"


int main(int argc, char *argv[]) {


    printf(" hola y mundo es '%s'\n",stringJoin("hola ", "mundo!!!"));

    if(argc != 3){
        printf("Expected:\n\t<program> [c|d] file\n");
        exit(1);
    }

    printf("Called with arguments:\n");
    int i;
    for (i = 0; i < argc; i++) {
        printf("\t%d: '%s'\n", i, argv[i]);
    }

    char *opt = argv[1];
    char *filename = argv[2];
    int filenamelen = strlen(filename);
    int maxOutFilenameLen = filenamelen + 5;
    char *newfilename = malloc(sizeof(char) * (maxOutFilenameLen));
    strncpy(newfilename, filename, maxOutFilenameLen);
    if(strcmp("opt","c")==0){
        strncat(newfilename,".rot",maxOutFilenameLen);
    }else{
        strncat(newfilename,".dec",maxOutFilenameLen);
    }

    printf("Sourcefile: '%s' dest: '%s'\n", filename, newfilename);



    if(strcmp(opt,"c")==0){         //code
        encodeRotFile(filename, newfilename);
    }else if(strcmp(opt,"d")==0){   // decode
        decodeRotFile(filename, newfilename);
    }else{
        printf("only get c or d as option\n");
    }
}



