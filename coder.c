
char encChar(char f){
    char encTab[256];
    encTab[' '] = 'a';
    encTab['#'] = 's';
    encTab['0'] = '2';
    encTab['1'] = '4';
    encTab['2'] = 'g';
    encTab['3'] = '5';
    encTab['4'] = '6';
    encTab['5'] = 'b';
    encTab['7'] = 'd';
    encTab['8'] = 'z';
    encTab['9'] = 'l';
    encTab['\n'] = 'f';
    return encTab[f];
}
char decChar(char f){
    char decTab[256];
    decTab['a'] = ' ';
    decTab['s'] = '#';
    decTab['2'] = '0';
    decTab['4'] = '1';
    decTab['g'] = '2';
    decTab['5'] = '3';
    decTab['6'] = '4';
    decTab['b'] = '5';
    decTab['d'] = '7';
    decTab['z'] = '8';
    decTab['l'] = '9';
    decTab['f'] ='\n';
    return decTab[f];
}


char *stringJoin(char *a, char *b){

    int maxLen = strlen(a) + strlen(b)+1;
    char *out=malloc(sizeof(char) * maxLen);
    strncat(out,a,maxLen);
    strncat(out,b,maxLen);
    return out;
}

void copyF(char *from, char *to){
    FILE *in;
    FILE *out;
    out=fopen(to,"w");
    in=fopen(from,"r");

    char f;
    while(fscanf(in,"%c",&f) != EOF){
        fprintf(out,"%c",f);
    }

    fclose(in);

    fclose(out);
}

void encodeRotFile(char *from, char *to){
    FILE *in;
    FILE *out;
    out=fopen(to,"w");

    in=fopen(from,"r");

    char f;
    while(fscanf(in,"%c",&f) != EOF){
        fprintf(out,"%c",encChar(f));
    }

    fclose(in);

    fclose(out);
}

void decodeRotFile(char *from, char *to){

    FILE *in;
    FILE *out;
    out=fopen(to,"w");

    in=fopen(from,"r");

    char f;
    while(fscanf(in,"%c",&f) != EOF){
        fprintf(out,"%c",decChar(f));
    }

    fclose(in);

    fclose(out);
}

void slurp(char *file){
    char *temp = stringJoin(file,"~");
    copyF(file, temp);
    encodeRotFile(temp, file);
}

void unslurp(char *file){
    char *temp = stringJoin(file,"~");
    copyF(file, temp);
    decodeRotFile(temp, file);
}




