char *stringAux;

int stringToInt(char t[]) {
    int res= 0, Ti= strlen(t)-1, piso= 0;

    if (t[0]== '-') {
        piso= 1;
    }

    for (; Ti>=piso; Ti--) {
        res+= (t[Ti]-48)*pow(10,strlen(t)-(Ti+1));
    }

    return (t[0]== '-')? -res : res;
}

int pow(int x, int y) {
    int Rstado=1;
    for (; y>0; y--) {
        Rstado *= x;
    }
    return Rstado;
}

char *reverseString(char string[]) {
    int i;
    char temp;

    for (i= 0; i < strlen(string)/2; i++) {
        temp= string[i];
        string[i]= string[strlen(string) -1 - i];
        string[strlen(string) -1 - i]= temp;
    }
    return string;
}

char *concatString(char s1[], char s2[]) {
    stringAux= (char *) calloc(1, 50);
    strcat(stringAux, s1);
    strcat(stringAux, " ");

    return strcat(stringAux, s2);;
}

char *joinString(char s1[], char s2[]) {
    stringAux= (char *) calloc(1, 50);
    strcat(stringAux, s1);
    return strcat(stringAux, s2);
}


char *strmplN(char *s1) {
    int i,j;

    for (i= 0; s1[i]!= 0; i++)
        if (s1[i] == 92 && s1[i+1] == 'n') {
            s1[i++]= '\n';
            for (j= i; s1[j] && s1[j+1]; j++) {
                s1[j]=s1[j+1];
            }
            s1[j]= 0;
        }

    return s1;
}

char *strmplT(char *s1) {
    int i,j;

    for (i= 0; s1[i]!= 0; i++)
        if (s1[i] == 92 && s1[i+1] == 't') {
            s1[i++]= '\t';
            for (j= i; s1[j] && s1[j+1]; j++) {
                s1[j]=s1[j+1];
            }
            s1[j]= 0;
        }

    return s1;
}



char *intToString(int num) {
    char *output= (char *)calloc (1, TAM_LEXEMA);
    int i=0;
    if (num >= 0) {
        if (num == 0) {
            output[0]= '0';
            i= 1;
        }
        for (; num>0; i++) {
            output[i]=num%10+48;
            num/=10;
        }
        output[i]= 0;
        return reverseString(output);
    } else {
        return joinString("-",intToString(-num));
    }
}
