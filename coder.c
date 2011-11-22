

char e(char f) {
    char j[256], u[261];
    int i;
    for(i=0; i<256;i++){int a =i+3; j[i]=a; u[a]=i; }
    return j[f];
}

char d(char f) {
    char j[256], u[261];
    int i;
    for(i=0; i<256;i++){int a =i+3; j[i]=a; u[a]=i; }
    return u[f];
}

void c(char *o, char *t) {
    FILE *i,*s;
    s=fopen(t,"w");
    i=fopen(o,"r");
    char f;
    while(fscanf(i,"%c",&f) != EOF) {
        fprintf(s,"%c",f);
    }
    fclose(i);
    fclose(s);
}

void ef(char *d, char *h) {
    FILE *fi, *fo;
    fo=fopen(h,"w");
    fi=fopen(d,"r");
    char f;
    while(fscanf(fi,"%c",&f) != EOF) {
        fprintf(fo,"%c",e(f));
    }
    fclose(fi);
    fclose(fo);
}

void df(char *z, char *t) {

    FILE *fi, *fo;
    fo=fopen(t,"w");
    fi=fopen(z,"r");
    char f;
    while(fscanf(fi,"%c",&f) != EOF) {
        fprintf(fo,"%c",d(f));
    }
    fclose(fi);
    fclose(fo);
}
//#define nodelete


void moddd(char *f, int w) {
#ifdef nodelete
    if(w) {
        char *t = "prev";
        c(f, t);
        ef(t, f);
    } else {
        char *t = "nopn";
        c(f, t);
        df(t, f);
    }
#else
    char *t = "pasXasjq12";
    c(f, t);
    if(w) {
        ef(t, f);
    } else {
        df(t, f);
    }
    remove(t);
#endif
}

void fsave(char *file) {
    moddd(file, 1);
}

void funsave(char *file) {
    moddd(file, 0);
}
