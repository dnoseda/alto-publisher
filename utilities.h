char *auxString;

int stringToInt(char t[]) {
    int res= 0, Ti= strlen(t)-1, piso= 0;

    if (t[0]== '-') {
        piso= 1;
    }

    for (; Ti>=piso; Ti--) {
        res+= (t[Ti]-48)*power(10,strlen(t)-(Ti+1));
    }

    return (t[0]== '-')? -res : res;
}

int power(int x, int y) {
    int Rstado=1;
    for (; y>0; y--) {
        Rstado *= x;
    }
    return Rstado;
}

char *stringReverse(char cadena[]) {
    int i;
    char temp;

    for (i= 0; i < strlen(cadena)/2; i++) {
        temp= cadena[i];
        cadena[i]= cadena[strlen(cadena) -1 - i];
        cadena[strlen(cadena) -1 - i]= temp;
    }
    return cadena;
}

char *concatString(char s1[], char s2[]) {
    auxString= (char *) calloc(1, 50);
    strcat(auxString, s1);
    strcat(auxString, " ");

    return strcat(auxString, s2);;
}

char *joinString(char s1[], char s2[]) {
    auxString= (char *) calloc(1, 50);
    strcat(auxString, s1);
    return strcat(auxString, s2);
}


char *unescapeNewline(char *s1) {
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

char *unescapeTab(char *s1) {
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
    char *result= (char *)calloc (1, TAM_LEXEMA);
    int i=0;
    if (num >= 0) {
        if (num == 0) {
            result[0]= '0';
            i= 1;
        }
        for (; num>0; i++) {
            result[i]=num%10+48;
            num/=10;
        }
        result[i]= 0;
        return stringReverse(result);
    } else {
        return joinString("-",intToString(-num));
    }
}
