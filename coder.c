char m(char f, int q){
    char j[256], u[261];
    int i;
    for(i=0; i<256;i++){int a =i+3; j[i]=a; u[a]=i; }
    return q ? u[f] : j[f];
}

void gg(char *o, char *t, int w){
    FILE *i,*s;
    s=fopen(t,"w");i=fopen(o,"r");
    char f;
    while(fscanf(i,"%c",&f) != EOF)
    switch(w){case 1: fprintf(s,"%c",f); break;
    case 2: fprintf(s,"%c",m(f,0)); break;
    case 3: fprintf(s,"%c",m(f,1)); break; }
    fclose(i);fclose(s);
}

void moddd(char *f, int w) {
    char *t = "pasXasjq12";
    gg(f, t, 1);
    if(w) gg(t, f,2);
    else gg(t, f,3);
    remove(t);
}

void fsave(char *file) {
    moddd(file, 1);
}

void funsave(char *file) {
    moddd(file, 0);
}
