/******************************************************************
            Este es el archivo correspondiente al PARSER
*******************************************************************/

#define EXTERNA extern

#include <stdlib.h>
#include <stdio.h>
#include "codigos.h"
#include "var_globales.h"
#include <string.h>
#include "ts.h"

#include "soporte_ejecucion.h"

#define TRUE (1 == 1)
#define FALSE (!TRUE)


// para representar el set
typedef long long set;

/*********** prototipos *************/

void unidad_traduccion();
void declaraciones();
void especificador_tipo();
void especificador_declaracion();
void definicion_funcion();
void declaracion_variable();
void lista_declaraciones_param();
void declaracion_parametro();
void declarador_init();
void lista_declacion_init();
struct Tipo constante();
void lista_inicializadores();
void lista_proposiciones();
void lista_declaraciones();
void declaracion();
void proposicion();
void proposicion_expresion();
void proposicion_compuesta();
void proposicion_seleccion();
void proposicion_iteracion();
void proposicion_e_s();
void proposicion_retorno();
struct Tipo variable();
struct Tipo expresion();
void expresion_asignacion();
void expresion_relacional();
struct Tipo expresion_simple();
void relacion();
struct Tipo termino();
struct Tipo factor();
struct Tipo llamada_funcion();
void lista_expresiones();
char *getStringINST(int INST);


void scanner ();

/********** variables globales ************/

//********************************************************




int isReturn= 0;
int isLlamadafuncion= 0;
int isINOUT= 0;
int llamolista_ini= 0;
int isConstante= 0;
int isdeffuncion= 0;
extern char error;
int vengodeIF =0;
int sentencia= 0;
int control= 0;

char *archivo;

char *codigo[15000]; //Para mostrar
char *codigoMostrar[15000]; //Para mostrar por pantalla
char *newLine;
int  newLineMAC= 0;

int constEntera= -1;
int cantConstantess= 0;
int cantParametros= 0;
int en_tabla_funcion= NIL;
int en_tabla_funcion_Llama= NIL;

int segVar= 2;

int punteroFuncion;  // donde apunto a la funcin donde estoy metido
int tipo_global;     // donde guardo en el caso     int a,b,c;    el tipo para b y c
int flag_hay_return; // dice si hay return en la declaracin de funcin
int void_flag;

/********** variables globales ************/


token *sbol;


int posID;
int posTabla;
int cantPar;
int tamARR=0;
char lexema[17];
int bandera;
int posicionTS;
int esParametro = FALSE;
tipo_inf_res *inicio; // puntero de la lista de parametros
tipo_inf_res *cursor; // cursor
char *archivo;


int esIndice = 0;

#define F_ESP_TIPO (CVOID|CCHAR|CINT|CFLOAT)
#define F_CONST (CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR)
#define F_LLAMADA_FUNC (CIDENT)
#define F_VAR (CIDENT)
#define F_REL (CIGUAL|CDISTINTO|CMENOR|CMAYOR|CMEIG|CMAIG)
#define F_PROP_E_S (CIN|COUT)
#define F_PROP_SEL (CIF)
#define F_PROP_IT (CWHILE)
#define F_PROP_RET (CRETURN)
#define F_DECL_INIT (CASIGNAC|CCOR_ABR)
#define F_LIST_DECL_INIT (CIDENT)
#define F_DEF_FUNC (CPAR_ABR)
#define F_PROP_COMP (CLLA_ABR)

#define F_DECL_PARAM (F_ESP_TIPO)
#define F_LIST_DECL_PARAM (F_DECL_PARAM)
#define F_DECL_VAR (F_DECL_INIT|CCOMA|CPYCOMA)
#define F_LIST_INIC (F_CONST)


#define F_FACTOR (F_VAR|F_CONST|CNEG|CPAR_ABR|F_LLAMADA_FUNC|CCONS_STR)
#define F_TERM (F_FACTOR)
#define F_EXP_SIMPLE (CMAS|CMENOS|F_TERM)
#define F_EXPR (F_EXP_SIMPLE)
#define F_LIST_EXPR (F_EXPR)
#define F_PROP_EXPR (F_EXPR|CPYCOMA)
#define F_DECL (F_ESP_TIPO)
#define F_LIST_DECL (F_DECL)
#define F_PROP (F_PROP_EXPR|F_PROP_COMP|F_PROP_SEL|F_PROP_IT|F_PROP_E_S|F_PROP_RET)
#define F_LIST_PROP (F_PROP)
#define F_ESP_DECLR (F_DEF_FUNC|F_DECL_VAR)
#define F_UNID_TRAD (F_DECL)


enum typeExpresion {
    variables,
    unaVariable,
    Constant,
    vars_consts,
    funcion,
    Const_iToStr
};


extern FILE *yyin;
extern int despl;

extern float P[];
extern int lp;
extern int lc;
extern char C[];
void scanner() {
    int i;
    for (; (i=yylex())!= NADA && sbol->codigo == SEGUIR;);
    if (i == NADA) {
        sbol->codigo = CEOF;
    }
    liberar = linea;
    linea = (char *) malloc (strlen(linea) + strlen (token1.lexema) + 3);
    strcpy(linea, liberar);
    strcat(linea, token1.lexema);
    free ( (void *) liberar);
}

//***************************funcionES DEL SET************************************


struct Tipo {
    enum    typeExpresion typeExpresionresion;
    int    tipo;
    int    tipo_base;
    float   valor;
    char    sValor[150];
    int     nivel;
    int     despl;
};


//*******************************************************************************
int toInt(char t[]) {
    int res= 0, Ti= strlen(t)-1, piso= 0;

    if (t[0]== '-') {
        piso= 1;
    }

    for (; Ti>=piso; Ti--) {
        res+= (t[Ti]-48)*elev(10,strlen(t)-(Ti+1));
    }

    return (t[0]== '-')? -res : res;
}

int elev(int x, int y) {
    int Rstado=1;
    for (; y>0; y--) {
        Rstado *= x;
    }
    return Rstado;
}

char *deReversa(char cadena[]) {
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
    newLine= (char *) calloc(1, 50);
    strcat(newLine, s1);
    strcat(newLine, " ");

    return strcat(newLine, s2);;
}

char *unionST(char s1[], char s2[]) {
    newLine= (char *) calloc(1, 50);
    strcat(newLine, s1);
    return strcat(newLine, s2);
}

//**********************************************************
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
//**********************************************************


char *iToStr(int num) {
    char *salida= (char *)calloc (1, TAM_LEXEMA);
    int i=0;
    if (num >= 0) {
        if (num == 0) {
            salida[0]= '0';
            i= 1;
        }
        for (; num>0; i++) {
            salida[i]=num%10+48;
            num/=10;
        }
        salida[i]= 0;
        return deReversa(salida);
    } else {
        return unionST("-",iToStr(-num));
    }
}

void appendMAC(int INST, char linea[]) {
    //printf("INSTI string:  %s\n", iToStr(INST));
    //printf("INSTI:  %d\n", INST);
    codigo[newLineMAC]= concatString(iToStr(INST),linea);
    //codigo[newLineMAC]= concatString(linea,iToStr(INST));
    codigoMostrar[newLineMAC++]= concatString(getStringINST(INST),linea);
}

void appendKMAC(int INST, char linea[], int kLinea) {
    int i;

    for (i= newLineMAC-1; i >= kLinea; i--) {
        codigo[i+1]= codigo[i];
        codigoMostrar[i+1]= codigoMostrar[i];
    }
    codigo[kLinea]= concatString(iToStr(INST),linea);
    codigoMostrar[kLinea]= concatString(getStringINST(INST),linea);

    newLineMAC++;
}

void appendParam(tipo_inf_res *info_param) {
    tipo_inf_res *cur;

    cur= ts[en_tabla_funcion].ets->desc.part_var.sub.ptr_inf_res;

    if (cur == NULL) {
        ts[en_tabla_funcion].ets->desc.part_var.sub.ptr_inf_res= info_param;
    } else {
        while (cur->ptr_sig != NULL) {
            cur= cur->ptr_sig;
        }

        cur->ptr_sig= info_param;
    }

}
tipo_inf_res getParam(int k) {
    int i;
    tipo_inf_res *cur, salida;

    cur= ts[en_tabla_funcion_Llama].ets->desc.part_var.sub.ptr_inf_res;

    for (i= 1; i <= k && cur != NULL; i++) {
        salida= *cur;
        cur= cur->ptr_sig;
    }
    return salida;
}

void chequeoParam(struct Tipo parametroReal, int numParametro) {
    tipo_inf_res parametroFormal;

    //  printf("numero de parametro..........       %d\n",numParametro);

    if (numParametro <= ts[en_tabla_funcion_Llama].ets->desc.part_var.sub.cant_par) {

        parametroFormal = getParam(numParametro);

        // printf("NUMERO PARAMETRO(%d)\n",numParametro);
        /*
        printf("%d\n",parametroFormal.ptero_tipo_base);
        printf("%d\n",parametroFormal.ptero_tipo);
        printf("%c\n",parametroFormal.tipo_pje);
        */

        if (parametroFormal.ptero_tipo == en_tabla("TIPOARREGLO")) {

            if (parametroReal.typeExpresionresion != unaVariable) {
                error_handler(91);
            } else if (parametroReal.tipo != en_tabla("TIPOARREGLO") || (parametroReal.tipo_base != parametroFormal.ptero_tipo_base)) {

                //printf("\n\n\nparametro real tipo... %d\n",parametroReal.tipo);
                //printf("posicion del tipo arreglo... %d\n",		en_tabla("TIPOARREGLO"));

                //printf("numero de parametro... %d\n\n\n\n\n",numParametro);
                //printf("parametro real tipo base... %d\n",parametroReal.tipo_base);
                //printf("parametro formal tipo base... %d\n",parametroFormal.ptero_tipo_base);

                //printf("arribaaaaaaaaaaaaa %d\n\n\n\n\n",numParametro);
                error_handler(90);
            }
        } else {

            if (parametroReal.tipo == en_tabla("TIPOARREGLO")) {
                error_handler(90);
            } else if (parametroFormal.tipo_pje == 'r' && parametroReal.typeExpresionresion != unaVariable) {
                error_handler(92);
            }

            if (parametroFormal.ptero_tipo == en_tabla("float") && (parametroReal.tipo == en_tabla("float") || parametroReal.tipo == en_tabla("char") || 				parametroReal.tipo == en_tabla("int"))) {
                return;
            } else //error_handler(90);

                if (parametroFormal.ptero_tipo == en_tabla("int") && (parametroReal.tipo == en_tabla("char") || parametroReal.tipo == en_tabla("int"))) {
                    return;
                } else //error_handler(90);
                    if (parametroFormal.ptero_tipo == en_tabla("char") && parametroReal.tipo == en_tabla("char") ) {
                        return;
                    } else {
                        error_handler(90);
                    }
        }


    }
}

float charToFloat(char num[]) {

    char part_ent[strlen(num)+1], part_dec[strlen(num)+1];
    int punto= 0;
    float res= 0;
    int i, decimales, piso= 0;


    if (num[0]== '-') {
        piso= 1;    //Si es negativo
    }

    part_ent[0]= part_dec[0]= '0';
    part_ent[1]= part_dec[1]= 0;

    for (i= piso; i <= strlen(num); i++) {
        if (num[i] == '.') {
            punto= 1;
            part_ent[i-piso]= 0;
        } else if (!punto) {
            part_ent[i-piso]= num[i];
        } else {
            part_dec[i-piso - (strlen(part_ent)+1)]= num[i];
        }
    }
    decimales= strlen(part_dec);
    res= (toInt(strcat(part_ent, part_dec))+.0)/elev(10, decimales);

    return (num[0]== '-')? -res : res;
}
void clearLMAC() {
    codigo[newLineMAC-1]= NULL;
    codigoMostrar[--newLineMAC]= NULL;
}

void clearKLMAC(int kLinea) {
    int i;

    codigo[kLinea]= NULL;
    codigoMostrar[kLinea]= NULL;

    for (i= kLinea; i < newLineMAC-1; i++) {
        codigo[i]= codigo[i+1];
        codigoMostrar[i]= codigoMostrar[i+1];
    }
    newLineMAC--;
}

void verInstrucciones() {
    int i;

    printf("\n\n MAC:\n\n\n");

    for (i= 0; i < newLineMAC; i++) {
        //printf("CODIGO %d: %s\n", i, codigo[i]);
        printf("Linea %d: %s\n", i+1, codigoMostrar[i]);
    }


    printf("\n******************\n");
}

/*
void generarSalida(){

printf("ffffffffffffffffff");
FILE *PObj;


if ((PObj= fopen(strcat(archivo, ".o"), "w")) != NULL){
int i;

for (i= 0; i < dameCS(); i++)
	fprintf(PObj, "%c\n", dameC(i));

//fprintf(PObj, "$ ");

for (i= 0; i < newLineMAC; i++)
	fprintf(PObj, "%s\n", codigoMostrar[i]);

for (i= 0; i < newLineMAC; i++)
	printf(dameC(i));

	}
fclose(PObj);
}
*/




void generarSalida() {
    FILE *PObj;
    char arreglo[500];
    int j;
    int t;
    int index,index2;
    char aux;
    char arreglo1[30];
    char arreglo2[30];
    int banderasa = 0;
    if ((PObj= fopen(strcat(archivo, ".o"), "w")) != NULL) {
        int i;
        //printf("dameCS:_________________ %c\n", dameCS());
        fprintf(PObj, "$ ");
        for (i= 0; i < newLineMAC; i++) {
            //index2=0;
            //strcpy(arreglo1,codigo[i]);
            //printf("arreglo: %s\n", arreglo1);
            /*for(index = 0; arreglo1[index]!='\0'; index++){
            	if(arreglo1[index]!= '_'){
            		arreglo2[index2++] = arreglo1[index];
            		banderasa=1;

            	}else{
            		banderasa=0;
                                    printf("arreglo2: %s\n", arreglo2);
                                    arreglo2[index2]='\0';
            		fprintf(PObj, "%f\n", charToFloat(arreglo2));
                                    index2=0; arreglo2[0]='\0';

            	}

            }*/
            //printf("arreglo2: %s\n", codigo[i]);
            fprintf(PObj, "%s\n", codigo[i]);
        }

        fprintf(PObj, "$ ");


        //for(t=0;t<150;t++) printf("arreglo");

        for (i= 0,j=0; i < dameCS(); i++) {
            //for (i= 0,j=0; i < dameCS()-1; i++){
            //aux = dameC(i);
            //printf("dameC metodo:               %c\n", dameC(i));
            //printf("aux:               %c\n", aux);
            //arreglo[j] = aux;
            fprintf(PObj, "%d\n", dameC(i));
            //fwrite(&aux, sizeof(char), 1,PObj);
        }
        fprintf(PObj, "$ ");
        //for(t=0;t<150;t++) printf("arreglo %c\n", arreglo[t]);
        //printf("arreglo %s\n", *arreglo);

        //aux =   "$";
        //fwrite(&aux, sizeof(char), 1,PObj);
    }
    fclose(PObj);
}

int tam_Instr(char *Inst) {
    int i, tam= 0;
    for (i= 0; i < strlen(Inst); i++)
        if (Inst[i] == ' ' && Inst[i+1] != 0) {
            tam++;
        }
    return tam + 1;
}

int calcularDespl(int LineaO, int LineaSalto) {
    int i, despl= 0;
    if (LineaO <= LineaSalto) {
        for (i= LineaO ; i < LineaSalto; i++) {
            despl+= tam_Instr(codigo[i]);
        }
        return despl;
    } else {
        return -(calcularDespl(LineaSalto, LineaO)+ 2 + 3);
    }
}



char Cohersion(char tipo, char Tipo_Operado) {
    char Tipo_Retorno= en_tabla("float");

    if (tipo == en_tabla("TIPOARREGLO") || Tipo_Operado == en_tabla("TIPOARREGLO")) {
        return en_tabla("TIPOARREGLO");
    }

    if (tipo == en_tabla("TIPOERROR") || Tipo_Operado == en_tabla("TIPOERROR")) {
        return en_tabla("TIPOERROR");
    }

    if (tipo == en_tabla("char")) {
        Tipo_Retorno= Tipo_Operado;
    } else if (tipo == en_tabla("int")) {
        if (Tipo_Operado == en_tabla("float")) {
            Tipo_Retorno= Tipo_Operado;
        } else {
            Tipo_Retorno= en_tabla("int");
        }
    }

    return Tipo_Retorno;

}


char getTipo(char tipo) {
    if (tipo == en_tabla("char")) {
        return 0;
    } else if (tipo == en_tabla("int")) {

        return 1;
    } else {

        return 2;
    }
}

char *getStringINST(int INST) {
    char *sINST= (char *)calloc (1, 13);
    strcpy(sINST, ">>ERROR<<");
    switch(INST) {
    case CRCT   :
        strcpy(sINST, sCRCT);
        break;
    case CRVL   :
        strcpy(sINST, sCRVL);
        break;
    case SUM    :
        strcpy(sINST, sSUM);
        break;
    case SUB    :
        strcpy(sINST, sSUB);
        break;
    case MUL    :
        strcpy(sINST, sMUL);
        break;
    case DIV    :
        strcpy(sINST, sDIV);
        break;
    case INV    :
        strcpy(sINST, sINV);
        break;
    case AND    :
        strcpy(sINST, sAND);
        break;
    case OR     :
        strcpy(sINST, sOR);
        break;
    case NEG    :
        strcpy(sINST, sNEG);
        break;
    case POP    :
        strcpy(sINST, sPOP);
        break;
    case CAST   :
        strcpy(sINST, sCAST);
        break;
    case CMMA   :
        strcpy(sINST, sCMMA);
        break;
    case CMME   :
        strcpy(sINST, sCMME);
        break;
    case CMIG   :
        strcpy(sINST, sCMIG);
        break;
    case CMAI   :
        strcpy(sINST, sCMAI);
        break;
    case CMEI   :
        strcpy(sINST, sCMEI);
        break;
    case CMNI   :
        strcpy(sINST, sCMNI);
        break;
    case ALM    :
        strcpy(sINST, sALM);
        break;
    case LEER   :
        strcpy(sINST, sLEER);
        break;
    case IMPR   :
        strcpy(sINST, sIMPR);
        break;
    case BIFF   :
        strcpy(sINST, sBIFF);
        break;
    case BIFS   :
        strcpy(sINST, sBIFS);
        break;
    case INPP   :
        strcpy(sINST, sINPP);
        break;
    case PARAR  :
        strcpy(sINST, sPARAR);
        break;
    case ALOC   :
        strcpy(sINST, sALOC);
        break;
    case DMEM   :
        strcpy(sINST, sDMEM);
        break;
    case CRDI   :
        strcpy(sINST, sCRDI);
        break;
    case CRVLI  :
        strcpy(sINST, sCRVLI);
        break;
    case ALMI   :
        strcpy(sINST, sALMI);
        break;
    case ENPR   :
        strcpy(sINST, sENPR);
        break;
    case CHPR   :
        strcpy(sINST, sCHPR);
        break;
    case RTPR   :
        strcpy(sINST, sRTPR);
        break;
    case ENBL   :
        strcpy(sINST, sENBL);
        break;
    case FINB   :
        strcpy(sINST, sFINB);
        break;
    case IMPCS  :
        strcpy(sINST, sIMPCS);
        break;
    case CRCTS  :
        strcpy(sINST, sCRCTS);
    }
    return sINST;
}

void compilacion() {

    sbol=&token1 ;

    appendMAC(INPP,"");
    //appendMAC(ENBL, iToStr(get_nivel()));

    printf("\nCOMPILACION\n");

    inic_tablas();

    scanner();

    unidad_traduccion(cons(NADA, CEOF));

    if (Clase_Ident("main") != CLASFUNC) {
        error_handler(15);
        error_handler(COD_IMP_ERRORES);
    } else if (ts[en_tabla("main")].ets->desc.part_var.sub.cant_par != 0) {
        error_handler(36);
        error_handler(COD_IMP_ERRORES);
    } else if (Tipo_Ident("main") != en_tabla("void")) {
        error_handler(35);
        error_handler(COD_IMP_ERRORES);
    }


    if (sbol->codigo != CEOF) {
        error_handler(8);
    }
    printf("\n\n\nCOMPILACION CORRECTA\n\n\n\n");
    //appendMAC(FINB, iToStr(get_nivel()));
    appendMAC(PARAR,"");
    if (error == 0) {
        verInstrucciones();
        generarSalida();
    }
}

/*
void ejecucion(){
    //int j =0;
    FILE *PObj;
    char cur[25];
    printf("archivo:   %s\n",strcat(archivo, ".o"));
    PObj= fopen(strcat(archivo, ".o"), "r");
    if (PObj!= NULL){
      //  printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
       // while(j<=1000000){
        // printf("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
        //  j++;
     //	}
        int i;
        fscanf(PObj, "%s", cur);
        //fgets (cur , 20,PObj);
        //printf("cur: %s",&cur);
       // for (i= 0; cur!=NULL ; i++){
        //for (i= 0; strcmp(cur, "$"); i++){
            addC(toInt(cur));
            fscanf(PObj, "%s", cur);
        }

       //for (i= 0; strcmp(cur, "$"); i++){
     //   addC(toInt(cur));
           // fscanf(PObj, "%s", cur);
     // }

     //    fscanf(PObj, "%s", cur);

     //        float val = -1;
     //        for (i = 0; !feof(PObj); i++) {
     //	       printf("cur: %s",cur);
     //            //val = P[i];
     //
     //              }

        // while(!feof(PObj)){
         for (i= 0;!feof(PObj); i++){
            float cod;

             cod= charToFloat(cur);
            //printf("floAT: %f",&cod);
	    //fgets (cur ,100,PObj);
            printf("cur: %s",cur);
            pushP(&cod);

            fscanf(PObj, "%s", cur);
        }
    }else{
          //printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
    }

    fclose(PObj);

    interprete();

}

*/


void ejecucion() {
    float cod;
    FILE *PObj;
    char cur[500];
    int kk;

    if ((PObj= fopen(strcat(archivo, ".o"), "r")) != NULL) {
        int i;
//      for(kk=0;kk<10000;kk++)printf("aaaaaaaaaaaaaaaaaaaaaaaaaaa\n");

        fscanf(PObj, "%s", &cur);
        fscanf(PObj, "%s", &cur);
        //for(kk=0;kk<10000;kk++) printf("PASOOOOOOOOOOOOO\n");
        for (i= 0; strcmp(cur, "$"); i++) {
            cod= charToFloat(cur);
            //  printf("COD: %f\n",cod);
            // printf("CUR: %s\n",cur);
            //pushP(&cod);                 // 1) Cargar Instruccion

            P[i] = cod;
            lp++;
            fscanf(PObj, "%s", &cur);    // 2) Leer otra Instruccion
        }

        // for(kk=0;kk<10000;kk++) printf("PASOOOOOOOOOOOOOFUERAAAAAAAAAA\n");

        fscanf(PObj, "%s", &cur);

        for (i= 0; strcmp(cur, "$"); i++) {  // 1) Leer y Cargar las Constanes de Strings
            addC(toInt(cur));
            fscanf(PObj, "%s", &cur);
        }

        /*
        float val = -1;
                        for (i = 0; val != PARAR; i++) {
                            fscanf(obj, "%f\n", &P[i]);
                            val = P[i];
             }
        */
        //for(kk=0;kk<200;kk++) printf("P: %f\n",P[kk]);
        //for(kk=0;kk<200;kk++) printf("C: %c\n",C[kk]);
    }
    fclose(PObj);
    //28352
    interprete2();
}

void test(set expected, set rec_points, int error) {
    if (sbol->codigo & expected) {
        return;
    }
    error_handler(error);
    set recover = expected | rec_points;
    while ((sbol->codigo & recover) == 0LL) {
        scanner();
    }
}



int main( int argc,char *argv[]) {

    linea = (char *) malloc (2);
    strcat(linea, "");

    nro_linea=0;

    if (argc != 3) {
        error_handler(6);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    } else {
        if ((yyin = fopen(argv[2], "r" )) == NULL) {
            error_handler(7);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        }
        archivo= argv[2];
        *(archivo + strlen(archivo)-2)= 0;
    }

    if (argv[1][1] == 'c') {
        compilacion();

    } else if (argv[1][1] == 'e') {

        ejecucion();

    }
}

/********* funciones del parser ***********/

void unidad_traduccion(set folset) {
    test(F_UNID_TRAD | folset, NADA, 50);
    while (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT || sbol->codigo == CFLOAT) {
        declaraciones(folset | F_DECL);
        test(F_DECL,folset,51);
    }
}

void declaraciones(set folset) {

    especificador_tipo(folset | CIDENT | F_ESP_DECLR);
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }
    especificador_declaracion(folset);
}

void especificador_tipo(set folset) {

    test(F_ESP_TIPO,folset,51);
    switch (sbol->codigo) {
    case CVOID: {
        scanner();
        posID=en_tabla("void");
        break;
    }
    case CCHAR: {
        scanner();
        posID=en_tabla("char");
        break;
    }
    case CINT: {
        scanner();
        posID=en_tabla("int");
        break;
    }
    case CFLOAT: {
        scanner();
        posID=en_tabla("float");
        break;
    }
    default:
        error_handler(17);
    }
    test(folset,NADA,52);
}

void especificador_declaracion(set folset) {
    test(F_ESP_DECLR,folset,53);
    switch (sbol->codigo) {
    case CPAR_ABR: {
        definicion_funcion(folset);
        break;
    }
    case CASIGNAC:
    case CCOR_ABR:
    case CCOMA:
    case CPYCOMA: {
        declaracion_variable(folset);
        break;
    }
    default:
        error_handler(18);
    }
    test(folset, NADA, 54);
}

void definicion_funcion(set folset) {
    isdeffuncion= 1;
    isReturn= 0;
    inf_id->clase = CLASFUNC;
    inf_id->ptr_tipo = posID;
    inf_id->cant_byte = ts[posID].ets->cant_byte;

    en_tabla_funcion= insertarTS();
    pushTB();

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    cantParametros= 0;

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT  || sbol->codigo == CFLOAT || sbol->codigo == CIDENT) {
        lista_declaraciones_param(CPAR_CIE | folset | F_PROP_COMP);
    }

    ts[en_tabla_funcion].ets->desc.part_var.sub.cant_par= cantParametros;

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion_compuesta(folset);
    en_tabla_funcion= NIL;
}

void lista_declaraciones_param(set folset) {

    declaracion_parametro(CCOMA | folset | F_DECL_PARAM);

    while (sbol->codigo ==CCOMA || sbol->codigo & F_DECL_PARAM) {
        if(sbol->codigo & F_DECL_PARAM) {
            error_handler(75);
        } else {
            scanner();
        }
        declaracion_parametro(CCOMA | folset | F_DECL_PARAM);
    }

    ts[posTabla].ets->desc.part_var.sub.cant_par=cantPar;
    ts[posTabla].ets->desc.part_var.sub.ptr_inf_res=inicio;

    inicio=NULL;
    cursor=NULL;
}

void lista_declaraciones_init(set folset) {

    test(F_LIST_DECL_INIT, folset | CCOMA | F_DECL_INIT, 57);
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }

    declarador_init(CCOMA  | CIDENT | F_DECL_INIT | folset);

    while (sbol->codigo == CCOMA || sbol->codigo == CIDENT) {

        if(sbol->codigo == CIDENT) {
            error_handler(75);
        } else {
            scanner();
        }
        if (sbol->codigo == CIDENT) {
            strcpy(inf_id->nbre,sbol->lexema);
            scanner();
        } else {
            error_handler(16);
        }

        declarador_init(CCOMA  | CIDENT | F_DECL_INIT | folset);
    }

}



void declaracion_parametro(set folset) {

    tipo_inf_res *info_res_param= NULL;
    info_res_param= (tipo_inf_res *) calloc(1, sizeof(tipo_inf_res));

    especificador_tipo(folset|CAMPER|CCOR_ABR|CCOR_CIE| CIDENT);

    inf_id->clase = CLASPAR;

    if (posID == en_tabla("void")) {
        error_handler(82);
        inf_id->ptr_tipo = en_tabla("TIPOERROR");
    } else {
        inf_id->ptr_tipo = posID;
    }
    if (sbol->codigo == CAMPER) {
        scanner();
        inf_id->desc.part_var.tipo_pje= 'r';
        inf_id->cant_byte = ts[en_tabla("int")].ets->cant_byte;
    } else {
        inf_id->desc.part_var.tipo_pje= 'v';
        inf_id->cant_byte = ts[posID].ets->cant_byte;
    }
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre, sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }
    if (sbol->codigo == CCOR_ABR) {
        if (inf_id->desc.part_var.tipo_pje == 'r') {
            error_handler(89);
        }

        inf_id->desc.part_var.tipo_pje == 'r';
        inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
        inf_id->ptr_tipo = en_tabla("TIPOARREGLO");

        scanner();
        if (sbol->codigo == CCOR_CIE) {
            scanner();
        } else {
            error_handler(21);
        }
    }

    cantParametros++;
    info_res_param->ptero_tipo= inf_id->ptr_tipo;
    info_res_param->tipo_pje= inf_id->desc.part_var.tipo_pje;
    info_res_param->ptero_tipo_base = inf_id->desc.part_var.arr.ptero_tipo_base ;
    info_res_param->ptr_sig= NULL;
    appendParam(info_res_param);
    insertarTS();
    test(folset,NADA, 55);
}

void declaracion_variable(set folset) {
    declarador_init(CCOMA|CPYCOMA | F_LIST_DECL_INIT|folset);

    if (sbol->codigo == CCOMA) {
        scanner();
        lista_declaraciones_init(folset | CPYCOMA);
    }
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
}



void declarador_init(set folset) {

    char t;
    struct Tipo TipoC;
    TipoC.tipo = NIL;
    test(F_DECL_INIT | folset, F_CONST, 58);

    if (posID == en_tabla("void")) {
        error_handler(82);
        inf_id->ptr_tipo = en_tabla("TIPOERROR");
    } else {
        inf_id->ptr_tipo = posID;
    }

    inf_id->cant_byte = ts[inf_id->ptr_tipo].ets->cant_byte;


    if (in(sbol->codigo,F_CONST)) {
        error_handler(79);
        constante(folset);
    } else
        switch (sbol->codigo) {
        case CASIGNAC:
            scanner();
            if(sbol->codigo==CCONS_STR) {
                scanner();
                error_handler(86);
            }
            TipoC= constante(folset);

            break;
        case CCOR_ABR:
            scanner();
            constEntera= -1;
            if (sbol->codigo == CCONS_ENT) {
                constEntera= toInt(sbol->lexema);
                if (constEntera <= 0) {
                    error_handler(80);
                }
                scanner();
            } else {
                scanner();
            }
            if (sbol->codigo == CCOR_CIE) {
                scanner();
            } else {
                error_handler(21);
            }

            if (sbol->codigo == CASIGNAC || sbol->codigo == CLLA_ABR) {
                if (sbol->codigo == CLLA_ABR) {
                    error_handler(79);
                } else {
                    scanner();
                }

                if (sbol->codigo == CLLA_ABR) {
                    scanner();
                } else {
                    error_handler(23);
                }

                cantConstantess= 0;
                lista_inicializadores(folset | CLLA_CIE);

                if (constEntera== -1) {
                    constEntera= cantConstantess;
                } else if (cantConstantess != constEntera) {
                    error_handler(94);
                }

                if (sbol->codigo == CLLA_CIE) {
                    scanner();
                } else {
                    error_handler(24);
                }
            }

            inf_id->desc.part_var.arr.cant_elem= constEntera;

            inf_id->desc.part_var.arr.ptero_tipo_base= inf_id->ptr_tipo;


            if (constEntera == -1) {
                error_handler(83);
                inf_id->desc.part_var.arr.ptero_tipo_base= en_tabla("TIPOERROR");
            }

            inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
            inf_id->cant_byte = (inf_id->desc.part_var.arr.cant_elem)*(ts[posID].ets->cant_byte);

            break;
        };

    inf_id->clase = CLASVAR;

    inf_id->desc.nivel = get_nivel();

    inf_id->desc.despl= despl;
    despl+= inf_id->cant_byte;

    appendMAC(ALOC, iToStr(inf_id->cant_byte));

    if (TipoC.tipo != NIL) {

        t= getTipo(inf_id->ptr_tipo);

        if (t == 2) {
            appendMAC(CRCT, concatString(iToStr(t), TipoC.sValor));
        } else {
            appendMAC(CRCT, concatString(iToStr(t), iToStr((int)TipoC.valor)));
        }
        appendMAC(ALM, concatString(concatString(iToStr(inf_id->desc.nivel),  iToStr(inf_id->desc.despl)), iToStr(t)));
        appendMAC(POP, iToStr(t));
    }

    insertarTS();
    test(folset, NADA, 59);
}

//MEEEEEEEEEEEEEEEEEEEEEEEEEEEE -dn

void lista_inicializadores(set folset) {  //OOOOOOOOOOOOOKKKKKKKKKKKKKKKKK
    llamolista_ini= 1;
    F_CONST
    constante(F_CONST | folset | CCOMA | NADA);
    llamolista_ini= 0;
    while (sbol->codigo == CCOMA||in(sbol->codigo,F_CONST)) {
        if(in(sbol->codigo,F_CONST)) {
            error_handler(75);
        } else {
            scanner();
        }
        constante(F_CONST | folset | CCOMA | NADA);
    }
}


void proposicion_compuesta(set folset) { //OOOOOOOOOOOOOOOOOOOOOOOOOKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
    int finBloqueVars;

    // F_PROP_COMP
    // F_LIST_DECL
    // F_LIST_PROP
    test(F_PROP_COMP, F_LIST_DECL | F_LIST_PROP | folset | CLLA_CIE | NADA,60);

    if (sbol->codigo == CLLA_ABR) {
        scanner();
    } else {
        error_handler(23);
    }

    if (!isdeffuncion) {
        pushTB();
    } else {
        isdeffuncion= 0;
    }

    appendMAC(ENBL, iToStr(get_nivel()));



    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        // printf("antes %s\n",sbol->lexema );
    {
        lista_declaraciones(folset | F_LIST_PROP | CLLA_CIE | NADA);
    }
    //printf("despues %s\n",sbol->lexema );
    finBloqueVars= newLineMAC;
    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN) {


        lista_proposiciones(folset | CLLA_CIE | NADA);
    }

    if (sbol->codigo == CLLA_CIE) {
        //printf("es llave que cierra %s\n",sbol->lexema );
        if (en_tabla_funcion != NIL && (ts[en_tabla_funcion].ets->ptr_tipo != en_tabla("void")) && (!isReturn)) {
            error_handler(37);
        }

        scanner();
    } else {
        error_handler(24);
    }

    appendMAC(FINB, iToStr(get_nivel()));
//    printf("antes del pop%s\n",sbol->lexema );
    pop_nivel();
    //  printf("despues del pop%s\n",sbol->lexema );
    test(folset, NADA | NADA, 61);
    //printf("antes del pop%s\n",sbol->lexema );
    return finBloqueVars;


}
void lista_declaraciones(set folset) { //OOOOOOOOOOOOKKKKKKKKKKKKKKKKKKKKKKKKKKKK

    // F_DECL
    declaracion(folset | F_DECL);

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        declaracion(folset | F_DECL);
    }

}

void declaracion(set folset) { //OOOOOOOOOOOOOOOOOOOOOOOOOOKKKKKKKKKKKKKKKKKKKKKKKKKK

    // F_LIST_DECL_INIT
    especificador_tipo(une(une(folset,F_LISTA_DECL_INIT),CPYCOMA | NADA));

    lista_declaraciones_init(folset | CPYCOMA | NADA);

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }


    test(folset,NADA | NADA,62);

}

void lista_proposiciones(set folset) { //OOOOOOOOOOOOKKKKKKKKKKKKKKKKKKKKK

    // F_PROP
    proposicion(une(folset,F_PROP));

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

    {
        proposicion(une(folset,F_PROP));
    }

}

void proposicion(set folset) { //VER EL RETUR, TIPO -1

    //if(vengodeIF)appendMAC(ENBL, iToStr(get_nivel()));

    test(F_PROP,folset,63);
    switch (sbol->codigo) {
    case CLLA_ABR:
        proposicion_compuesta(folset);
        break;
    case CWHILE:
        proposicion_iteracion(folset);
        break;
    case CIF:
        proposicion_seleccion(folset);
        break;
    case CIN:
    case COUT:
        proposicion_e_s(folset);
        break;
    case CMAS:
    case CMENOS:
    case CIDENT:
    case CPAR_ABR:
    case CNEG:
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
    case CCONS_STR:
    case CPYCOMA:
        proposicion_expresion(folset);
        break;
    case CRETURN:
        proposicion_retorno(folset);
        break;
    default:
        error_handler(25);

        //if(vengodeIF){appendMAC(FINB, iToStr(get_nivel()));vengodeIF = 0;}

    }
    return -1;
}

void proposicion_iteracion(set folset) {// OKKKKKKKKKKKKKKKKKKKK



    if (sbol->codigo == CWHILE) {
        scanner();
    } else {
        error_handler(26);
    }

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    expresion(une(une(folset,F_PROP),CPAR_CIE | NADA));

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion(folset);

}


void proposicion_seleccion(set folset) { // AGREGARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR

    struct Tipo TipoEx;
    int lineaBIFF, lineaBIFS, d1;
    if (sbol->codigo == CIF) {
        scanner();
    } else {
        error_handler(27);
    }

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    TipoEx=expresion(une(une(folset,F_PROP),cons(CPAR_CIE|CELSE,NADA)));



    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }


    lineaBIFF= newLineMAC;    //guardala linea del if
// printf("newLineMac1: %d \n", newLineMAC);
    //vengodeIF = 1;
    proposicion(une(CELSE | NADA | folset,F_PROP));

    d1 = calcularDespl(lineaBIFF, newLineMAC); //calcula el desplazamianto a donde tiene que ir a para el BIFF



    if (sbol->codigo == CELSE) {
        lineaBIFS= newLineMAC;  //guardala linea del else
        scanner();
//printf("newLineMac2: %d \n", newLineMAC);
        proposicion(folset);
        //printf("newLineMac2: %d \n", newLineMAC);
        d1= calcularDespl(lineaBIFS, newLineMAC);

        appendKMAC(BIFS, iToStr(d1), lineaBIFS);

        d1= calcularDespl(lineaBIFF, lineaBIFS + 1);

    }
    // printf("newLineMac3: %d \n", newLineMAC);
    appendKMAC(BIFF, concatString(iToStr(getTipo(TipoEx.tipo)), iToStr(d1)), lineaBIFF);
}


void proposicion_e_s(set folset) { // // noooooooooooooooooooooooooooooooooooooooooooooo

    struct Tipo TipoExp;
    char t;

    isINOUT= 1;

    switch(sbol->codigo) {
    case CIN: {
        scanner();
        if (sbol->codigo == CSHR) {
            scanner();
        } else {
            error_handler(28);
        }
        //F_VAR
        TipoExp= variable(une(une(folset,cons(CSHR|CPYCOMA,NADA)),F_VAR));

        clearLMAC();

        t= getTipo(TipoExp.tipo);

        appendMAC(LEER, iToStr(getTipo(TipoExp.tipo)));
        appendMAC(ALM, concatString(concatString(iToStr(TipoExp.nivel),  iToStr(TipoExp.despl)), iToStr(t)));
        appendMAC(POP, iToStr(t));



        while (sbol->codigo == CSHR || in(sbol->codigo,F_VAR)) {
            if (in(sbol->codigo, F_VAR)) {
                error_handler(76);
            } else {
                scanner();
            }

            TipoExp= variable(une(une(folset,cons(CSHR|CPYCOMA,NADA)),F_VAR));

            clearLMAC();

            t= getTipo(TipoExp.tipo);

            appendMAC(LEER, iToStr(getTipo(TipoExp.tipo)));
            appendMAC(ALM, concatString(concatString(iToStr(TipoExp.nivel),  iToStr(TipoExp.despl)), iToStr(t)));
            appendMAC(POP, iToStr(t));
        }

        if (sbol->codigo == CPYCOMA) {
            scanner();
        } else {
            error_handler(22);
        }
        break;
    }
    case COUT: {
        scanner();
        if (sbol->codigo == CSHL) {
            scanner();
        } else {
            error_handler(29);
        }


        TipoExp= expresion(une(une(folset,cons(CSHL|CPYCOMA,NADA)),first(expresio)));
        if (TipoExp.typeExpresionresion == Const_iToStr) {
            appendMAC(IMPCS,"");
        } else {
            appendMAC(IMPR, iToStr(getTipo(TipoExp.tipo)));
        }

        while (sbol->codigo == CSHL || in(sbol->codigo,first(expresio))) {
            if(in(sbol->codigo,first(expresio))) {
                error_handler(77);
            } else {
                scanner();
            }
            TipoExp= expresion(une(une(folset,cons(CSHL|CPYCOMA,NADA)),first(expresio)));
            if (TipoExp.typeExpresionresion == Const_iToStr) {
                appendMAC(IMPCS, "");
            } else {
                appendMAC(IMPR, iToStr(getTipo(TipoExp.tipo)));
            }

        }
        if (sbol->codigo == CPYCOMA) {
            scanner();
        } else {
            error_handler(22);
        }
        break;
    }
    default:
        error_handler(30);
    }
    test(folset,NADA | NADA,64);
    isINOUT= 0;
}


void proposicion_retorno(set folset) {	//OKKKKKKKKKKKKKK
    //bandera = 1;
    scanner();
    expresion(folset | CPYCOMA | NADA);
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    isReturn= 1;
    test(folset,NADA | NADA,65);
}


void proposicion_expresion(set folset) { //OKKKKKKKKKKKKKK
    sentencia= 1;
    control= -1;

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

    {
        expresion(folset | CPYCOMA | NADA);
    }

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    test(folset,NADA | NADA,66);
    sentencia= 0;
}

struct Tipo expresion(set folset) {
    struct Tipo Tipo_Retorno,TipoE;
    char tvar,t;
    int nLineaCast;
    long int op;

    control++;

    Tipo_Retorno=   expresion_simple(une(une(folset,first(expresio)),cons(CASIGNAC, CDISTINTO|CIGUAL|CMENOR|CMEIG|CMAYOR|CMAIG)));
    nLineaCast= newLineMAC;

    TipoE.tipo= en_tabla("char");

    if (in(sbol->codigo, first(expresio))) {
        error_handler(78);
        TipoE= expresion(folset);
    } else
        switch (sbol->codigo) {

        case CASIGNAC:
            scanner();
            if (Tipo_Retorno.typeExpresionresion != unaVariable) {
                error_handler(84);
            }

            clearLMAC();

            TipoE= expresion(folset);

            if (TipoE.tipo > Tipo_Retorno.tipo) {
                error_handler(93);
            }

            Tipo_Retorno.valor= TipoE.valor;

            tvar= getTipo(Tipo_Retorno.tipo);
            t= getTipo(TipoE.tipo);

            if (t != tvar) {
                appendMAC(CAST,concatString( iToStr(t), iToStr(tvar)));
            }

            appendMAC(ALM, concatString(concatString(iToStr(Tipo_Retorno.nivel),  iToStr(Tipo_Retorno.despl)), iToStr(tvar)));
            //
            appendMAC(POP, iToStr(tvar));
            appendMAC(CRVL, concatString(concatString(iToStr(Tipo_Retorno.nivel),  iToStr(Tipo_Retorno.despl)), iToStr(tvar)));
            //
            if (sentencia && control == 0) {
                appendMAC(POP, iToStr(tvar));
            }

            break;
        case CDISTINTO:
        case CIGUAL:
        case CMENOR:
        case CMEIG:
        case CMAYOR:
        case CMAIG:
            op= sbol->codigo;

            scanner();
            TipoE= expresion(folset);

            t= getTipo(Tipo_Retorno.tipo);
            tvar= getTipo(TipoE.tipo);


            if (tvar < t) {
                appendMAC(CAST, concatString(iToStr(tvar), iToStr(t)));
            } else if (tvar > t) {
                appendKMAC(CAST, concatString(iToStr(t), iToStr(tvar)), nLineaCast);
                t= tvar;
            }

            switch(op) {
            case CDISTINTO:
                appendMAC(CMNI, iToStr(t));
                break;
            case CIGUAL:
                appendMAC(CMIG, iToStr(t));
                break;
            case CMENOR:
                appendMAC(CMME, iToStr(t));
                //appendMAC(CMMA, iToStr(t));
                break;
            case CMEIG:
                appendMAC(CMEI, iToStr(t));
                break;
            case CMAYOR:
                appendMAC(CMMA, iToStr(t));
                //appendMAC(CMME, iToStr(t));
                break;
            case CMAIG:
                appendMAC(CMAI, iToStr(t));
            }
            break;
        };

    Tipo_Retorno.tipo= Cohersion(Tipo_Retorno.tipo,TipoE.tipo);
    control--;

    return Tipo_Retorno;
}



struct Tipo expresion_simple(set folset) {
    struct Tipo TipoT, Tipo_Retorno;
    int masMenos= 0;
    long int op= CMAS;
    char t, tvar;
    int nLineaCast;

    test(first(expresion_simpl),une(une(first(termin),NADA | COR),folset),67);

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
        op= sbol->codigo;
        scanner();
        masMenos= 1;
    }

    Tipo_Retorno= termino(une(une(folset,cons(NADA,CMAS|CMENOS|COR)),first(termin)));
    nLineaCast= newLineMAC;


    if (op == CMENOS) {
        Tipo_Retorno.valor = -Tipo_Retorno.valor;
        appendMAC(INV, iToStr(getTipo(Tipo_Retorno.tipo)));
    }

    if (Tipo_Retorno.typeExpresionresion == unaVariable && masMenos) {
        Tipo_Retorno.typeExpresionresion= variables;
    }

    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR || in(sbol->codigo, first(termin))) {
        op= sbol->codigo;

        if (in(sbol->codigo, first(termin))) {
            error_handler(78);
        } else {
            scanner();
        }
        TipoT= termino(une(une(folset,cons(NADA,CMAS|CMENOS|COR)),first(termin)));

        t= getTipo(Tipo_Retorno.tipo);
        tvar= getTipo(TipoT.tipo);


        if (tvar < t) {
            appendMAC(CAST, concatString(iToStr(tvar), iToStr(t)));
        } else if (tvar > t) {
            appendKMAC(CAST, concatString(iToStr(t), iToStr(tvar)), nLineaCast);
            t= tvar;
        }


        switch(op) {
        case CMAS:
            appendMAC(SUM, iToStr(t));
            break;
        case CMENOS:
            appendMAC(SUB, iToStr(t));
            break;
        case COR:
            appendMAC(OR, iToStr(t));
        }


        Tipo_Retorno.tipo= Cohersion(Tipo_Retorno.tipo, TipoT.tipo);
        Tipo_Retorno.typeExpresionresion= (Tipo_Retorno.typeExpresionresion == unaVariable ||
                                           (TipoT.typeExpresionresion != Tipo_Retorno.typeExpresionresion))?
                                          vars_consts : TipoT.typeExpresionresion;

        if (Tipo_Retorno.typeExpresionresion == Constant && (Tipo_Retorno.tipo == en_tabla("int") || Tipo_Retorno.tipo == en_tabla("char"))) {
            switch(op) {
            case CMAS:
                Tipo_Retorno.valor+= TipoT.valor;
                break;
            case CMENOS:
                Tipo_Retorno.valor-= TipoT.valor;
                break;
            case COR:
                Tipo_Retorno.valor= Tipo_Retorno.valor || TipoT.valor;
            }
        }
        nLineaCast= newLineMAC;
    }

    return Tipo_Retorno;
}

struct Tipo termino(set folset) {  // OKKKKKKKKKKKKKK
    long int op;
    struct Tipo Tipo_Retorno, TipoF;
    char t, tvar;
    int nLineaCast;

    Tipo_Retorno= factor(une(une(folset,cons(NADA,CMULT|CDIV|CAND)),first(facto)));
    nLineaCast= newLineMAC;




    if (Tipo_Retorno.typeExpresionresion == variables) {
        Tipo_Retorno.typeExpresionresion= unaVariable;
    }

    while ((sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)|| in(sbol->codigo,first(facto))) {
        op= sbol->codigo;
        if(in(sbol->codigo,first(facto))) {
            error_handler(78);
        } else {
            scanner();
        }
        TipoF= factor(une(une(folset,cons(NADA,CMULT|CDIV|CAND)),first(facto)));

        t= getTipo(Tipo_Retorno.tipo);
        tvar= getTipo(TipoF.tipo);

        if (tvar < t) {
            appendMAC(CAST, concatString(iToStr(tvar), iToStr(t)));
        } else if (tvar > t) {
            appendKMAC(CAST, concatString(iToStr(t), iToStr(tvar)), nLineaCast);
            t= tvar;
        }



        switch(op) {
        case CMULT:
            appendMAC(MUL, iToStr(t));
            break;
        case CDIV:
            appendMAC(DIV, iToStr(t));
            break;
        case CAND:
            appendMAC(AND, iToStr(t));
        }

        Tipo_Retorno.tipo= Cohersion(Tipo_Retorno.tipo, TipoF.tipo);
        Tipo_Retorno.typeExpresionresion= (TipoF.typeExpresionresion != Tipo_Retorno.typeExpresionresion)? vars_consts : TipoF.typeExpresionresion;

        if (Tipo_Retorno.typeExpresionresion == Constant && (Tipo_Retorno.tipo == en_tabla("int") || Tipo_Retorno.tipo == en_tabla("char"))) {
            switch(op) {
            case CMULT:
                Tipo_Retorno.valor*= TipoF.valor;
                break;
            case CDIV:
                Tipo_Retorno.valor= (int)(Tipo_Retorno.valor / TipoF.valor);
                break;
            case CAND:
                Tipo_Retorno.valor= Tipo_Retorno.valor && TipoF.valor;
            }
        }
        nLineaCast= newLineMAC;
    }

    return Tipo_Retorno;

}

struct Tipo factor(set folset) {//0KKKKKKKKKKKKKKKK

    char lexema[TAM_LEXEMA], t;
    struct Tipo Tipo_Retorno;
    int i,j;
    char* s1;

    test(first(facto), folset, 68);

    switch (sbol->codigo) {
    case CIDENT:
        if (Clase_Ident(sbol->lexema) == NIL) {

            strcpy(lexema, sbol->lexema);

            scanner();

            error_handler(33);

            if (sbol->codigo == CPAR_ABR) {

                strcpy(inf_id->nbre, lexema);
                inf_id->clase = CLASFUNC;
                inf_id->ptr_tipo = en_tabla("TIPOERROR");

                scanner();

                if (sbol->codigo == CMAS      || sbol->codigo == CMENOS    ||
                        sbol->codigo == CIDENT    || sbol->codigo == CPAR_ABR  ||
                        sbol->codigo == CNEG      || sbol->codigo == CCONS_ENT ||
                        sbol->codigo == CCONS_FLO || sbol->codigo == CCONS_CAR ||
                        sbol->codigo == CCONS_STR) {
                    lista_expresiones(une(folset, cons(CPAR_CIE, NADA)));
                }

                if (sbol->codigo == CPAR_CIE) {
                    scanner();
                } else {
                    error_handler(20);
                }

            } else {
                strcpy(inf_id->nbre, lexema);
                inf_id->clase = CLASVAR;
                inf_id->ptr_tipo = en_tabla("TIPOERROR");


                if (sbol->codigo == CCOR_ABR) {

                    inf_id->desc.part_var.arr.ptero_tipo_base= inf_id->ptr_tipo;
                    inf_id->ptr_tipo = en_tabla("TIPOARREGLO");

                    scanner();
                    expresion(une(folset, cons(CCOR_CIE, NADA)));

                    if (sbol->codigo == CCOR_CIE) {
                        scanner();
                    } else {
                        error_handler(21);
                    }
                }

            }
            insertarTS();

        } else if (Clase_Ident(sbol->lexema) == CLASFUNC) {
            Tipo_Retorno= llamada_funcion(folset);
        } else {
            Tipo_Retorno= variable(folset);
        }

        break;
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
        Tipo_Retorno= constante(folset);
        t= getTipo(Tipo_Retorno.tipo);
        if (t == 2) {
            appendMAC(CRCT, concatString(iToStr(t),  Tipo_Retorno.sValor));
        } else {
            appendMAC(CRCT, concatString(iToStr(t), Tipo_Retorno.sValor));
        }
        break;
    case CCONS_STR:
        if (!isINOUT) {
            error_handler(86);
        }

        appendMAC(CRCTS, iToStr(dameCS()));
        /*
        s1=sbol->lexema;
        for (i= 0; s1[i]!= 0; i++)
           				if (s1[i] == 91 && s1[i+1] == 'n'){
               					s1[i++]= '\n';
               					for (j= i; s1[j] && s1[j+1]; j++)
                   				s1[j]=s1[j+1];
               					s1[j]= 0;
           				}
        				for (i= 0; s1[i]!= 0; i++)
           				if (s1[i] == 91 && s1[i+1] == 't'){
               					s1[i++]= '\t';
               					for (j= i; s1[j] && s1[j+1]; j++)
                   				s1[j]=s1[j+1];
               					s1[j]= 0;
           				}
                           *sbol->lexema=s1;
                           */
        strmplN(sbol->lexema);
        strmplT(sbol->lexema);

        pushCS(sbol->lexema);

        Tipo_Retorno.typeExpresionresion= Const_iToStr;
        strcpy(Tipo_Retorno.sValor, sbol->lexema);
        scanner();
        break;
    case CPAR_ABR:
        scanner();
        Tipo_Retorno= expresion(une(folset, cons(CPAR_CIE, NADA)));

        if (sbol->codigo == CPAR_CIE) {
            scanner();
        } else {
            error_handler(20);
        }

        break;
    case CNEG:
        scanner();
        Tipo_Retorno= expresion(folset);
        t= getTipo(Tipo_Retorno.tipo);

        appendMAC(NEG, iToStr(t));
        break;
    default:
        error_handler(31);
        Tipo_Retorno.tipo= en_tabla("TIPOERROR");
    }

    test(folset, NADA | NADA, 69);

    return Tipo_Retorno;
}


struct Tipo variable(set folset) {
    char lexema[TAM_LEXEMA], t;
    int Iden_No_Declarado= 0;
    struct Tipo TipoE, Tipo_Retorno;


    test(F_VAR, folset, 70);

    if (sbol->codigo == CIDENT) {

        strcpy(lexema, sbol->lexema);

        Iden_No_Declarado= (Clase_Ident(sbol->lexema) == NIL)? 1 : 0;

        scanner();

        if (Iden_No_Declarado) {

            error_handler(33);

            if (sbol->codigo == CPAR_ABR) {

                strcpy(inf_id->nbre, lexema);
                inf_id->clase = CLASFUNC;
                inf_id->ptr_tipo = en_tabla("TIPOERROR");

                scanner();

                if (sbol->codigo == CMAS 	  || sbol->codigo == CMENOS    ||
                        sbol->codigo == CIDENT    || sbol->codigo == CPAR_ABR  ||
                        sbol->codigo == CNEG      || sbol->codigo == CCONS_ENT ||
                        sbol->codigo == CCONS_FLO || sbol->codigo == CCONS_CAR ||
                        sbol->codigo == CCONS_STR) {
                    lista_expresiones(une(folset, cons(CPAR_CIE, NADA)));
                }

                if (sbol->codigo == CPAR_CIE) {
                    scanner();
                } else {
                    error_handler(20);
                }

            } else {
                strcpy(inf_id->nbre, lexema);
                inf_id->clase = CLASVAR;
                inf_id->ptr_tipo = en_tabla("TIPOERROR");


                if (sbol->codigo == CCOR_ABR) {

                    inf_id->desc.part_var.arr.ptero_tipo_base= inf_id->ptr_tipo;
                    inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
                    inf_id->desc.part_var.arr.cant_elem= 99999; //inicializo con un nro grande

                    scanner();
                    expresion(une(folset, cons(CCOR_CIE, NADA)));

                    if (sbol->codigo == CCOR_CIE) {
                        scanner();
                    } else {
                        error_handler(21);
                    }
                }

            }

            insertarTS();

        } else {
            if (sbol->codigo == CCOR_ABR) {
                scanner();
                TipoE= expresion(une(folset, cons(CCOR_CIE, NADA)));

                if (TipoE.typeExpresionresion == Constant) {
                    if (TipoE.tipo == en_tabla("float") || TipoE.valor < 0) {
                        error_handler(85);
                    } else if (TipoE.valor >= ts[en_tabla(lexema)].ets->desc.part_var.arr.cant_elem) {
                        error_handler(85);
                    }
                }

                if (sbol->codigo == CCOR_CIE) {
                    scanner();
                } else {
                    error_handler(21);
                }


                if (Tipo_Ident(lexema) != en_tabla("TIPOARREGLO")) {
                    error_handler(32);
                } else {
                    Tipo_Retorno.tipo= ts[en_tabla(lexema)].ets->desc.part_var.arr.ptero_tipo_base;
                    //printf("retorno de base arreglo: %d\n",Tipo_Retorno.tipo);
                    //printf("lexema arreglo: %s\n",lexema);
                }
            } else if (Tipo_Ident(lexema) == en_tabla("TIPOARREGLO")&&!isLlamadafuncion) {
                error_handler(43);
            } else {
                //Tipo_Retorno.tipo= Tipo_Ident(lexema)+0x30;
                Tipo_Retorno.tipo= Tipo_Ident(lexema);
                //printf("retorno de tipo_ident: %d\n",Tipo_Retorno.tipo);
                //printf("lexema.......................iuiuip: %s\n",lexema);
                t= getTipo(Tipo_Retorno.tipo);

                appendMAC(CRVL, concatString(concatString(iToStr(ts[en_tabla(lexema)].ets->desc.nivel), iToStr(ts[en_tabla(lexema)].ets->desc.despl)), iToStr(t)));

                if (Tipo_Retorno.tipo == en_tabla("TIPOARREGLO")) {
                    Tipo_Retorno.tipo_base= ts[en_tabla(lexema)].ets->desc.part_var.arr.ptero_tipo_base;
                }
            }
        }

        Tipo_Retorno.typeExpresionresion= variables;
        Tipo_Retorno.despl= ts[en_tabla(lexema)].ets->desc.despl;
        Tipo_Retorno.nivel= ts[en_tabla(lexema)].ets->desc.nivel;
    } else {
        error_handler(16);
    }

    test(folset, NADA | NADA, 71);

    return Tipo_Retorno;
}


struct Tipo llamada_funcion(set folset) {
    char lexema[TAM_LEXEMA];
    struct Tipo Tipo_Retorno;
    isLlamadafuncion= 1;

    en_tabla_funcion_Llama= en_tabla(sbol->lexema);

    if (sbol->codigo == CIDENT) {
        strcpy(lexema, sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    cantParametros= 0;

    if (sbol->codigo == CMAS 	  || sbol->codigo == CMENOS    ||
            sbol->codigo == CIDENT    || sbol->codigo == CPAR_ABR  ||
            sbol->codigo == CNEG      || sbol->codigo == CCONS_ENT ||
            sbol->codigo == CCONS_FLO || sbol->codigo == CCONS_CAR ||
            sbol->codigo == CCONS_STR) {
        lista_expresiones(une(folset, cons(CPAR_CIE, NADA)));
    }

    if (cantParametros != ts[en_tabla(lexema)].ets->desc.part_var.sub.cant_par) {
        error_handler(88);
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    isLlamadafuncion= 0;

    test(folset, NADA | NADA, 72);

    Tipo_Retorno.tipo= Tipo_Ident(lexema);
    Tipo_Retorno.typeExpresionresion= funcion;

    return Tipo_Retorno;
}


void lista_expresiones(set folset) { //0KKK
    struct Tipo TipoE;
    cantParametros= 0;

    //F_EXPR
    TipoE= expresion(une(folset | CCOMA | NADA,first(expresio)));
    cantParametros++;


    /*
       printf("int: %d\n", TipoE.typeExpresionresion);
       printf("char: %c\n", TipoE.tipo);
       printf("char: %c\n", TipoE.tipo_base);
       printf("float: %f\n", TipoE.valor);
       printf("string: %s\n", TipoE.sValor);
       printf("nivel: %d\n", TipoE.nivel);
       printf("despl: %d\n", TipoE.despl);

    */


    chequeoParam(TipoE, cantParametros);//cheque el tipo de expresion TipoE con el parametro cantParametros

    while (sbol->codigo == CCOMA || in(sbol->codigo, first(expresio))) {
        if (in(sbol->codigo, first(expresio))) {
            error_handler(75);
        } else {
            scanner();
        }

        TipoE= expresion(une(folset | CCOMA | NADA,first(expresio)));
        cantParametros++;

        chequeoParam(TipoE, cantParametros);
    }
}

struct Tipo constante(set folset) {    // ver el ultimo case
    struct Tipo Tipo_Retorno;

    Tipo_Retorno.typeExpresionresion= Constant;

    test(F_CONST,folset,73);
    switch (sbol->codigo) {
    case CCONS_ENT:
//printf("retorno de base arreglo: %d\n",Tipo_Retorno.tipo);
        //printf("lexema arreglo: %s\n",lexema);
        Tipo_Retorno.valor= atoi(sbol->lexema);
        strcpy(Tipo_Retorno.sValor, sbol->lexema);

        cantConstantess++;
        scanner();
        Tipo_Retorno.tipo= en_tabla("int");
        break;
    case CCONS_FLO:
        Tipo_Retorno.valor= charToFloat(sbol->lexema);
        strcpy(Tipo_Retorno.sValor, sbol->lexema);

        cantConstantess++;
        scanner();
        Tipo_Retorno.tipo= en_tabla("float");
        break;
    case CCONS_CAR:
        Tipo_Retorno.valor= sbol->lexema[1];
        strcpy(Tipo_Retorno.sValor, iToStr(Tipo_Retorno.valor));

        cantConstantess++;
        scanner();
        Tipo_Retorno.tipo= en_tabla("char");
        break;
    default:
        error_handler(38);

    }
    test(folset,NADA | NADA,74);
    return Tipo_Retorno;

}

/*

void imprimirPmt(){
int i = 0;
tipo_inf_res *cursorCroto;
cursorCroto = inicio;
while(cursorCroto!=NULL){

	printf("(%d)\n",i);
	printf("%d\n",cursorCroto->ptero_tipo);
	printf("%c\n",cursorCroto->tipo_pje);
	i++;
	cursorCroto=cursorCroto->ptr_sig;
}

}
*/

