# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# define MAX_LINES 100
# define MAX_CHARS 10

typedef struct{
    char code[MAX_LINES][MAX_CHARS];

}Instr;

int main(){

    Instr i;
    printf("Hola Mundo\n");
    strncpy(i.code[0],"Hola",MAX_CHARS);

    printf("El valor es '%s'\n",i.code[0]);


    FILE *outFile;

    /* open the file we are writing to */
    if(!(outFile = fopen("binout", "w"))){
        return 1;
    }

    /* use fwrite to write binary data to the file */
    fwrite(&i, sizeof(Instr), 1, outFile);

    fclose(outFile);



    printf("levanto archivo\n");

    FILE *inFile;

    if(!(inFile = fopen("binout", "r"))){
        return 1;
    }

    Instr o;
    Instr *p;
    p=&o;

    fread((Instr*)p, sizeof(Instr),1,inFile);

    printf("contenido de archivo '%s'", o.code[0]);

    fclose(inFile);

}
