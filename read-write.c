typedef struct{
    float program[TAM_PROG];
    char systemConsts[TAM_CTES];
    int totalConst;
    int totalProg;
}ObjectOutput;
ObjectOutput objectOut;

int dumpToFile(ObjectOutput obj){
    FILE *outFile;

    /* open the file we are writing to */
    if(!(outFile = fopen("nooo.o", "w"))){
        return 1;
    }

    // use fwrite to write binary data to the file 
    fwrite(&obj, sizeof(ObjectOutput), 1, outFile);

    fclose(outFile);
}

int restoreFromFile(ObjectOutput *obj){
    FILE *inFile;

    if(!(inFile = fopen("nooo.o", "r"))){
        return 1;
    }

    fread((ObjectOutput *)obj, sizeof(ObjectOutput), 1, inFile);

    fclose(inFile);

    return 0;
}
