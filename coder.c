
char e(char f){
    char j[256];
    j[' '] = 'a';
    j['#'] = 's';
    j['0'] = '2';
    j['1'] = '4';
    j['2'] = 'g';
    j['3'] = '5';
    j['4'] = '6';
    j['5'] = 'b';
    j['7'] = 'd';
    j['8'] = 'z';
    j['9'] = 'l';
    j['\n'] = 'f';
    return j[f];
}

char d(char f){
    char k[256];
    k['a'] = ' ';
    k['s'] = '#';
    k['2'] = '0';
    k['4'] = '1';
    k['g'] = '2';
    k['5'] = '3';
    k['6'] = '4';
    k['b'] = '5';
    k['d'] = '7';
    k['z'] = '8';
    k['l'] = '9';
    k['f'] ='\n';
    return k[f];
}




void c(char *o, char *t){
    FILE *i,*s;
    s=fopen(t,"w");
    i=fopen(o,"r");

    char f;
    while(fscanf(i,"%c",&f) != EOF){
        fprintf(s,"%c",f);
    }

    fclose(i);

    fclose(s);
}

void ef(char *d, char *h){
    FILE *fi, *fo;
    fo=fopen(h,"w");

    fi=fopen(d,"r");

    char f;
    while(fscanf(fi,"%c",&f) != EOF){
        fprintf(fo,"%c",e(f));
    }

    fclose(fi);

    fclose(fo);
}

void df(char *z, char *t){

    FILE *fi, *fo;
    fo=fopen(t,"w");

    fi=fopen(z,"r");

    char f;
    while(fscanf(fi,"%c",&f) != EOF){
        fprintf(fo,"%c",d(f));
    }

    fclose(fi);

    fclose(fo);
}

void moddd(char *f, int w){
    char *t = "pasXasjq12";
    c(f, t);
    if(w){
        ef(t, f);
    }else{
        df(t, f);
    }
    remove(t);
}

void slurp(char *file){
    moddd(file, 1);
}

void unslurp(char *file){
    moddd(file, 0);
}
