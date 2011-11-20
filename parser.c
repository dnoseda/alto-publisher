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
#include "utilities.h"

#include "soporte_ejecucion.h"
#include "coder.c"
#include "first.c"

#define TRUE (1 == 1)
#define FALSE (!TRUE)

//#define DEBUG 1

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
struct TipoAttr constante();
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
struct TipoAttr variable();
struct TipoAttr expresion();
void expresion_asignacion();
void expresion_relacional();
struct TipoAttr expresion_simple();
void relacion();
struct TipoAttr termino();
struct TipoAttr factor();
struct TipoAttr llamada_funcion();
void lista_expresiones();
char *getStringINST(int INST);


void scanner ();

/********** variables globales ************/

int isIOSentence= 0;
int  indexMAC= 0;
int isReturnSentence= 0;
int constantQuantity= 0;
extern char error;
int call_Lista_Init= 0;
int intConstant= -1;
int parameterQuantity= 0;
int inTableFunction= NIL;
int isControlSentence= 0;
int isSentence= 0;
int isFunctionInvocation= 0;
int inInvocation= NIL;
char *archivo;

int isFunctionDefinition= 0;

/********** variables globales ************/

/** FORMAC **/
# define MAX_INSTR 5000
char *outputCode[MAX_INSTR];

char *outputCodeToShow[MAX_INSTR];
/**/


int parameterQuantity;
tipo_inf_res *cursor;
int idPosition;

tipo_inf_res *start;
int tablePosition;
char *archivo;
token *sbol;


enum ExpresionType {
    aVariable,constOfExpresionType,bunchOfVariables, constVariable, function,constIntToString
};

extern FILE *yyin;
extern int despl;

extern float P[];
extern int lp;


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




struct TipoAttr {
    char    stringValue[150]; int    intType; int    baseType;
    float   value;    int     level;  int     despl;
    enum    ExpresionType expresionType;
};


void insertMAC(int INST, char linea[]) {
    outputCode[indexMAC]= stringConcat(intToString(INST),linea);
    outputCodeToShow[indexMAC++]= stringConcat(getStringINST(INST),linea);
}

void insertKMAC(int INST, char linea[], int kLinea) {
    int i;

    for (i= indexMAC-1; i >= kLinea; i--) {
        outputCode[i+1] = outputCode[i];
        outputCodeToShow[i+1] = outputCodeToShow[i];
    }
    outputCode[kLinea]= stringConcat(intToString(INST),linea);
    outputCodeToShow[kLinea]= stringConcat(getStringINST(INST),linea);

    indexMAC++;
}

void addParam(tipo_inf_res *info_param) {
    tipo_inf_res *cur;

    cur= ts[inTableFunction].ets->desc.part_var.sub.ptr_inf_res;

    if (cur == NULL) {
        ts[inTableFunction].ets->desc.part_var.sub.ptr_inf_res= info_param;
    } else {
        while (cur->ptr_sig != NULL) {
            cur= cur->ptr_sig;
        }

        cur->ptr_sig= info_param;
    }

}

tipo_inf_res getParameter(int position) {
    int i;
    tipo_inf_res *cur, output;

    cur= ts[inInvocation].ets->desc.part_var.sub.ptr_inf_res;

    for (i= 1; i <= position && cur != NULL; i++) {
        output= *cur;
        cur= cur->ptr_sig;
    }
    return output;
}

void paramChecking(struct TipoAttr current, int paramQuantity) {
    tipo_inf_res formalParameter;

    if (paramQuantity <= ts[inInvocation].ets->desc.part_var.sub.cant_par) {

        formalParameter = getParameter(paramQuantity);

        if (formalParameter.ptero_tipo == en_tabla("TIPOARREGLO")) {

            if (current.expresionType != aVariable) {
                error_handler(91);
            } else if (current.intType != en_tabla("TIPOARREGLO") || (current.baseType != formalParameter.ptero_tipo_base)) {
                error_handler(90);
            }
        } else {

            if (current.intType == en_tabla("TIPOARREGLO")) {
                error_handler(90);
            } else if (formalParameter.tipo_pje == 'r' && current.expresionType != aVariable) {
                error_handler(92);
            }

            if (formalParameter.ptero_tipo == en_tabla("float") && (current.intType == en_tabla("float") || current.intType == en_tabla("char") || 				current.intType == en_tabla("int"))) {
                return;
            } else{
                if (formalParameter.ptero_tipo == en_tabla("int") && (current.intType == en_tabla("char") || current.intType == en_tabla("int"))) {
                    return;
                } else if (formalParameter.ptero_tipo == en_tabla("char") && current.intType == en_tabla("char") ) {
                    return;
                } else {
                    error_handler(90);
                }
            }
        }
    }
}


void clearLMAC() {
    outputCode[indexMAC-1]= NULL;
    outputCodeToShow[--indexMAC]= NULL;
}

void clearKLMAC(int kLinea) {
    int i;

    outputCode[kLinea]= NULL;
    outputCodeToShow[kLinea]= NULL;

    for (i= kLinea; i < indexMAC-1; i++) {
        outputCode[i]= outputCode[i+1];
        outputCodeToShow[i]= outputCodeToShow[i+1];
    }
    indexMAC--;
}

void verInstrucciones() {
#ifdef DEBUG
    int i;

    printf("instructions:\n\n\n");

    for (i= 0; i < indexMAC; i++) {
        printf("[%d]: %s\n", i+1, outputCodeToShow[i]);
    }

#endif
}


void generateObjectFile() {
    FILE *PObj;
        
    int i;
    for (i= 0; i < dameCS(); i++) {
    }

    /**/

    char *filename=strcat(archivo, ".o");
    if ((PObj= fopen(filename, "w")) != NULL) {

        fprintf(PObj, "### ");
        for (i= 0; i < indexMAC; i++) {
            fprintf(PObj, "%s\n", outputCode[i]);
        }
        fprintf(PObj, "### ");
        for (i= 0; i < dameCS(); i++) {
            fprintf(PObj, "%d\n", dameC(i));
        }
        fprintf(PObj, "### ");
    }
    fclose(PObj);

    slurp(filename);
    /**/
}


char Cohersion(char tipo, char Tipo_Operado) {
    char resultingType= en_tabla("float");

    if (tipo == en_tabla("TIPOARREGLO") || Tipo_Operado == en_tabla("TIPOARREGLO")) {
        return en_tabla("TIPOARREGLO");
    }

    if (tipo == en_tabla("TIPOERROR") || Tipo_Operado == en_tabla("TIPOERROR")) {
        return en_tabla("TIPOERROR");
    }

    if (tipo == en_tabla("char")) {
        resultingType= Tipo_Operado;
    } else if (tipo == en_tabla("int")) {
        if (Tipo_Operado == en_tabla("float")) {
            resultingType= Tipo_Operado;
        } else {
            resultingType= en_tabla("int");
        }
    }

    return resultingType;

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


int getInstructionSize(char *Inst) {
    int i, tam= 0;
    for (i= 0; i < strlen(Inst); i++)
        if (Inst[i] == ' ' && Inst[i+1] != 0) {
            tam++;
        }
    return tam + 1;
}

int getOffset(int LineaO, int LineaSalto) {
    int i, despl= 0;
    if (LineaO <= LineaSalto) {
        for (i= LineaO ; i < LineaSalto; i++) {
            despl+= getInstructionSize(outputCode[i]);
        }
        return despl;
    } else {
        return -(getOffset(LineaSalto, LineaO)+ 2 + 3);
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



void execution() {

    int i;
    float cod;

    /**/    
    FILE *PObj;
    char cur[500];    

    char *filename = strcat(archivo, ".o");
    unslurp(filename);
    if ((PObj= fopen(filename, "r")) != NULL) {


        fscanf(PObj, "%s", cur);
        fscanf(PObj, "%s", cur);

        for (i= 0; strcmp(cur, "###"); i++) {
            cod= charToFloat(cur);
            P[i] = cod;
            lp++;
            fscanf(PObj, "%s", cur);            
        }

        fscanf(PObj, "%s", cur);

        for (i= 0; strcmp(cur, "###"); i++) {
            addC(stringToInt(cur));
            fscanf(PObj, "%s", cur);
        }


    }
    fclose(PObj);
    slurp(filename);
    /**/

    interprete2();
}

void compilation() {

    sbol=&token1 ;

    insertMAC(INPP,"");


    printf("\nCompilacion:\n");

    inic_tablas();

    scanner();

    unidad_traduccion(NADA |  CEOF);

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
    printf("\n...compilado!\n");

    insertMAC(PARAR,"");
    if (error == 0) {
        verInstrucciones();
        generateObjectFile();
    }
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
        compilation();

    } else if (argv[1][1] == 'e') {

        execution();

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
        idPosition=en_tabla("void");
        break;
    }
    case CCHAR: {
        scanner();
        idPosition=en_tabla("char");
        break;
    }
    case CINT: {
        scanner();
        idPosition=en_tabla("int");
        break;
    }
    case CFLOAT: {
        scanner();
        idPosition=en_tabla("float");
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
    isFunctionDefinition= 1;
    isReturnSentence= 0;
    inf_id->clase = CLASFUNC;
    inf_id->ptr_tipo = idPosition;
    inf_id->cant_byte = ts[idPosition].ets->cant_byte;

    inTableFunction= insertarTS();
    pushTB();

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    parameterQuantity= 0;

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT  || sbol->codigo == CFLOAT || sbol->codigo == CIDENT) {
        lista_declaraciones_param(CPAR_CIE | folset | F_PROP_COMP);
    }

    ts[inTableFunction].ets->desc.part_var.sub.cant_par= parameterQuantity;

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion_compuesta(folset);
    inTableFunction= NIL;
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

    ts[tablePosition].ets->desc.part_var.sub.cant_par=parameterQuantity;
    ts[tablePosition].ets->desc.part_var.sub.ptr_inf_res=start;

    start=NULL;
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

    if (idPosition == en_tabla("void")) {
        error_handler(82);
        inf_id->ptr_tipo = en_tabla("TIPOERROR");
    } else {
        inf_id->ptr_tipo = idPosition;
    }
    if (sbol->codigo == CAMPER) {
        scanner();
        inf_id->desc.part_var.tipo_pje= 'r';
        inf_id->cant_byte = ts[en_tabla("int")].ets->cant_byte;
    } else {
        inf_id->desc.part_var.tipo_pje= 'v';
        inf_id->cant_byte = ts[idPosition].ets->cant_byte;
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

    parameterQuantity++;
    info_res_param->ptero_tipo= inf_id->ptr_tipo;
    info_res_param->tipo_pje= inf_id->desc.part_var.tipo_pje;
    info_res_param->ptero_tipo_base = inf_id->desc.part_var.arr.ptero_tipo_base ;
    info_res_param->ptr_sig= NULL;
    addParam(info_res_param);
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
    struct TipoAttr TipoC;
    TipoC.intType = NIL;
    test(F_DECL_INIT | folset, F_CONST, 58);

    if (idPosition == en_tabla("void")) {
        error_handler(82);
        inf_id->ptr_tipo = en_tabla("TIPOERROR");
    } else {
        inf_id->ptr_tipo = idPosition;
    }

    inf_id->cant_byte = ts[inf_id->ptr_tipo].ets->cant_byte;


    if (sbol->codigo & F_CONST) {
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
            intConstant= -1;
            if (sbol->codigo == CCONS_ENT) {
                intConstant= stringToInt(sbol->lexema);
                if (intConstant <= 0) {
                    error_handler(81);
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

                constantQuantity= 0;
                lista_inicializadores(folset | CLLA_CIE);

                if (intConstant== -1) {
                    intConstant= constantQuantity;
                } else if (constantQuantity != intConstant) {
                    error_handler(94);
                }

                if (sbol->codigo == CLLA_CIE) {
                    scanner();
                } else {
                    error_handler(24);
                }
            }

            inf_id->desc.part_var.arr.cant_elem= intConstant;

            inf_id->desc.part_var.arr.ptero_tipo_base= inf_id->ptr_tipo;


            if (intConstant == -1) {
                error_handler(83);
                inf_id->desc.part_var.arr.ptero_tipo_base= en_tabla("TIPOERROR");
            }

            inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
            inf_id->cant_byte = (inf_id->desc.part_var.arr.cant_elem)*(ts[idPosition].ets->cant_byte);

            break;
        };

    inf_id->clase = CLASVAR;

    inf_id->desc.nivel = get_nivel();

    inf_id->desc.despl= despl;
    despl+= inf_id->cant_byte;

    insertMAC(ALOC, intToString(inf_id->cant_byte));

    if (TipoC.intType != NIL) {

        t= getTipo(inf_id->ptr_tipo);

        if (t == 2) {
            insertMAC(CRCT, stringConcat(intToString(t), TipoC.stringValue));
        } else {
            insertMAC(CRCT, stringConcat(intToString(t), intToString((int)TipoC.value)));
        }
        insertMAC(ALM, stringConcat(stringConcat(intToString(inf_id->desc.nivel),  intToString(inf_id->desc.despl)), intToString(t)));
        insertMAC(POP, intToString(t));
    }

    insertarTS();
    test(folset, NADA, 59);
}



void lista_inicializadores(set folset) {
    call_Lista_Init= 1;
    constante(F_CONST | folset | CCOMA | NADA);
    call_Lista_Init= 0;
    while (sbol->codigo == CCOMA||(sbol->codigo & F_CONST)) {
        if((sbol->codigo & F_CONST)) {
            error_handler(75);
        } else {
            scanner();
        }
        constante(F_CONST | folset | CCOMA | NADA);
    }
}


void proposicion_compuesta(set folset) {
    int finBloqueVars;

    test(F_PROP_COMP, F_LIST_DECL | F_LIST_PROP | folset | CLLA_CIE | NADA,60);

    if (sbol->codigo == CLLA_ABR) {
        scanner();
    } else {
        error_handler(23);
    }

    if (!isFunctionDefinition) {
        pushTB();
    } else {
        isFunctionDefinition= 0;
    }

    insertMAC(ENBL, intToString(get_nivel()));



    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)


    {
        lista_declaraciones(folset | F_LIST_PROP | CLLA_CIE | NADA);
    }

    finBloqueVars= indexMAC;
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

        if (inTableFunction != NIL && (ts[inTableFunction].ets->ptr_tipo != en_tabla("void")) && (!isReturnSentence)) {
            error_handler(37);
        }

        scanner();
    } else {
        error_handler(24);
    }

    insertMAC(FINB, intToString(get_nivel()));

    pop_nivel();

    test(folset, NADA | NADA, 61);


}
void lista_declaraciones(set folset) {

    declaracion(folset | F_DECL);

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        declaracion(folset | F_DECL);
    }

}

void declaracion(set folset) {

    especificador_tipo(folset | F_LIST_DECL_INIT | CPYCOMA | NADA);

    lista_declaraciones_init(folset | CPYCOMA | NADA);

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }


    test(folset,NADA | NADA,62);

}

void lista_proposiciones(set folset) {

    proposicion(folset | F_PROP);

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

    {
        proposicion(folset | F_PROP);
    }

}

void proposicion(set folset) {



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



    }
}

void proposicion_iteracion(set folset) {



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

    expresion(folset | F_PROP | CPAR_CIE | NADA);

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion(folset);

}


void proposicion_seleccion(set folset) {

    struct TipoAttr TipoEx;
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

    TipoEx=expresion(folset | F_PROP | CPAR_CIE|CELSE | NADA);



    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }


    lineaBIFF= indexMAC;


    proposicion(CELSE | NADA | folset | F_PROP);

    d1 = getOffset(lineaBIFF, indexMAC);



    if (sbol->codigo == CELSE) {
        lineaBIFS= indexMAC;
        scanner();

        proposicion(folset);

        d1= getOffset(lineaBIFS, indexMAC);

        insertKMAC(BIFS, intToString(d1), lineaBIFS);

        d1= getOffset(lineaBIFF, lineaBIFS + 1);

    }

    insertKMAC(BIFF, stringConcat(intToString(getTipo(TipoEx.intType)), intToString(d1)), lineaBIFF);
}


void proposicion_e_s(set folset) {

    struct TipoAttr TipoExp;
    char t;

    isIOSentence= 1;

    switch(sbol->codigo) {
    case CIN: {
        scanner();
        if (sbol->codigo == CSHR) {
            scanner();
        } else {
            error_handler(28);
        }

        TipoExp= variable(folset | CSHR|CPYCOMA | NADA | F_VAR);

        clearLMAC();

        t= getTipo(TipoExp.intType);

        insertMAC(LEER, intToString(getTipo(TipoExp.intType)));
        insertMAC(ALM, stringConcat(stringConcat(intToString(TipoExp.level),  intToString(TipoExp.despl)), intToString(t)));
        insertMAC(POP, intToString(t));



        while (sbol->codigo == CSHR || (sbol->codigo & F_VAR)) {
            if ((sbol->codigo & F_VAR)) {
                error_handler(76);
            } else {
                scanner();
            }

            TipoExp= variable(folset | CSHR|CPYCOMA | NADA | F_VAR);

            clearLMAC();

            t= getTipo(TipoExp.intType);

            insertMAC(LEER, intToString(getTipo(TipoExp.intType)));
            insertMAC(ALM, stringConcat(stringConcat(intToString(TipoExp.level),  intToString(TipoExp.despl)), intToString(t)));
            insertMAC(POP, intToString(t));
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


        TipoExp= expresion(folset | CSHL|CPYCOMA | NADA | F_EXPR);
        if (TipoExp.expresionType == constIntToString) {
            insertMAC(IMPCS,"");
        } else {
            insertMAC(IMPR, intToString(getTipo(TipoExp.intType)));
        }

        while (sbol->codigo == CSHL || (sbol->codigo & F_EXPR)) {
            if(sbol->codigo & F_EXPR) {
                error_handler(77);
            } else {
                scanner();
            }
            TipoExp= expresion(folset | CSHL|CPYCOMA | NADA | F_EXPR);
            if (TipoExp.expresionType == constIntToString) {
                insertMAC(IMPCS, "");
            } else {
                insertMAC(IMPR, intToString(getTipo(TipoExp.intType)));
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
    isIOSentence= 0;
}


void proposicion_retorno(set folset) {

    scanner();
    expresion(folset | CPYCOMA | NADA);
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    isReturnSentence= 1;
    test(folset,NADA | NADA,65);
}


void proposicion_expresion(set folset) {
    isSentence= 1;
    isControlSentence= -1;

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
    isSentence= 0;
}

struct TipoAttr expresion(set folset) {
    struct TipoAttr Tipo_Retorno,TipoE;
    char tvar,t;
    int nLineaCast;
    long int op;

    isControlSentence++;

    Tipo_Retorno=   expresion_simple(folset | F_EXPR | CASIGNAC |  CDISTINTO|CIGUAL|CMENOR|CMEIG|CMAYOR|CMAIG);
    nLineaCast= indexMAC;

    TipoE.intType= en_tabla("char");

    if ((sbol->codigo & F_EXPR)) {
        error_handler(78);
        TipoE= expresion(folset);
    } else
        switch (sbol->codigo) {

        case CASIGNAC:
            scanner();
            if (Tipo_Retorno.expresionType != aVariable) {
                error_handler(84);
            }

            clearLMAC();

            TipoE= expresion(folset);

            if (TipoE.intType > Tipo_Retorno.intType) {
                error_handler(93);
            }

            Tipo_Retorno.value= TipoE.value;

            tvar= getTipo(Tipo_Retorno.intType);
            t= getTipo(TipoE.intType);

            if (t != tvar) {
                insertMAC(CAST,stringConcat( intToString(t), intToString(tvar)));
            }

            insertMAC(ALM, stringConcat(stringConcat(intToString(Tipo_Retorno.level),  intToString(Tipo_Retorno.despl)), intToString(tvar)));

            insertMAC(POP, intToString(tvar));
            insertMAC(CRVL, stringConcat(stringConcat(intToString(Tipo_Retorno.level),  intToString(Tipo_Retorno.despl)), intToString(tvar)));

            if (isSentence && isControlSentence == 0) {
                insertMAC(POP, intToString(tvar));
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

            t= getTipo(Tipo_Retorno.intType);
            tvar= getTipo(TipoE.intType);


            if (tvar < t) {
                insertMAC(CAST, stringConcat(intToString(tvar), intToString(t)));
            } else if (tvar > t) {
                insertKMAC(CAST, stringConcat(intToString(t), intToString(tvar)), nLineaCast);
                t= tvar;
            }

            switch(op) {
            case CDISTINTO:
                insertMAC(CMNI, intToString(t));
                break;
            case CIGUAL:
                insertMAC(CMIG, intToString(t));
                break;
            case CMENOR:
                insertMAC(CMME, intToString(t));

                break;
            case CMEIG:
                insertMAC(CMEI, intToString(t));
                break;
            case CMAYOR:
                insertMAC(CMMA, intToString(t));

                break;
            case CMAIG:
                insertMAC(CMAI, intToString(t));
            }
            break;
        };

    Tipo_Retorno.intType= Cohersion(Tipo_Retorno.intType,TipoE.intType);
    isControlSentence--;

    return Tipo_Retorno;
}



struct TipoAttr expresion_simple(set folset) {
    struct TipoAttr TipoT, Tipo_Retorno;
    int masMenos= 0;
    long int op= CMAS;
    char t, tvar;
    int nLineaCast;



    test(F_EXP_SIMPLE,F_TERM | NADA | COR | folset,67);

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
        op= sbol->codigo;
        scanner();
        masMenos= 1;
    }

    Tipo_Retorno= termino(folset | NADA | CMAS|CMENOS|COR | F_TERM);
    nLineaCast= indexMAC;


    if (op == CMENOS) {
        Tipo_Retorno.value = -Tipo_Retorno.value;
        insertMAC(INV, intToString(getTipo(Tipo_Retorno.intType)));
    }

    if (Tipo_Retorno.expresionType == aVariable && masMenos) {
        Tipo_Retorno.expresionType= bunchOfVariables;
    }

    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR || (sbol->codigo & F_TERM)) {
        op= sbol->codigo;

        if ((sbol->codigo & F_TERM)) {
            error_handler(78);
        } else {
            scanner();
        }
        TipoT= termino(folset | NADA | CMAS|CMENOS|COR | F_TERM);

        t= getTipo(Tipo_Retorno.intType);
        tvar= getTipo(TipoT.intType);


        if (tvar < t) {
            insertMAC(CAST, stringConcat(intToString(tvar), intToString(t)));
        } else if (tvar > t) {
            insertKMAC(CAST, stringConcat(intToString(t), intToString(tvar)), nLineaCast);
            t= tvar;
        }


        switch(op) {
        case CMAS:
            insertMAC(SUM, intToString(t));
            break;
        case CMENOS:
            insertMAC(SUB, intToString(t));
            break;
        case COR:
            insertMAC(OR, intToString(t));
        }


        Tipo_Retorno.intType= Cohersion(Tipo_Retorno.intType, TipoT.intType);
        Tipo_Retorno.expresionType= (Tipo_Retorno.expresionType == aVariable ||
                                           (TipoT.expresionType != Tipo_Retorno.expresionType))?
                                          constVariable : TipoT.expresionType;

        if (Tipo_Retorno.expresionType == constOfExpresionType && (Tipo_Retorno.intType == en_tabla("int") || Tipo_Retorno.intType == en_tabla("char"))) {
            switch(op) {
            case CMAS:
                Tipo_Retorno.value+= TipoT.value;
                break;
            case CMENOS:
                Tipo_Retorno.value-= TipoT.value;
                break;
            case COR:
                Tipo_Retorno.value= Tipo_Retorno.value || TipoT.value;
            }
        }
        nLineaCast= indexMAC;
    }

    return Tipo_Retorno;
}

struct TipoAttr termino(set folset) {
    long long op;
    struct TipoAttr Tipo_Retorno, TipoF;
    char t, tvar;
    int nLineaCast;


    Tipo_Retorno= factor(folset | NADA | CMULT|CDIV|CAND | F_FACTOR);
    nLineaCast= indexMAC;




    if (Tipo_Retorno.expresionType == bunchOfVariables) {
        Tipo_Retorno.expresionType= aVariable;
    }

    while ((sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)|| (sbol->codigo & F_FACTOR)) {
        op= sbol->codigo;
        if((sbol->codigo & F_FACTOR)) {
            error_handler(78);
        } else {
            scanner();
        }
        TipoF= factor(folset | NADA | CMULT|CDIV|CAND | F_FACTOR);

        t= getTipo(Tipo_Retorno.intType);
        tvar= getTipo(TipoF.intType);

        if (tvar < t) {
            insertMAC(CAST, stringConcat(intToString(tvar), intToString(t)));
        } else if (tvar > t) {
            insertKMAC(CAST, stringConcat(intToString(t), intToString(tvar)), nLineaCast);
            t= tvar;
        }

        switch(op) {
        case CMULT:
            insertMAC(MUL, intToString(t));
            break;
        case CDIV:
            insertMAC(DIV, intToString(t));
            break;
        case CAND:
            insertMAC(AND, intToString(t));
        }

        Tipo_Retorno.intType= Cohersion(Tipo_Retorno.intType, TipoF.intType);
        Tipo_Retorno.expresionType= (TipoF.expresionType != Tipo_Retorno.expresionType)? constVariable : TipoF.expresionType;

        if (Tipo_Retorno.expresionType == constOfExpresionType && (Tipo_Retorno.intType == en_tabla("int") || Tipo_Retorno.intType == en_tabla("char"))) {
            switch(op) {
            case CMULT:
                Tipo_Retorno.value*= TipoF.value;
                break;
            case CDIV:
                Tipo_Retorno.value= (int)(Tipo_Retorno.value / TipoF.value);
                break;
            case CAND:
                Tipo_Retorno.value= Tipo_Retorno.value && TipoF.value;
            }
        }
        nLineaCast= indexMAC;
    }

    return Tipo_Retorno;

}

struct TipoAttr factor(set folset) {

    char lexema[TAM_LEXEMA], t;
    struct TipoAttr Tipo_Retorno;
    int i,j;
    char* s1;

    test(F_FACTOR, folset, 68);

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
                    lista_expresiones(folset |  CPAR_CIE |  NADA);
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
                    expresion(folset |  CCOR_CIE |  NADA);

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
        t= getTipo(Tipo_Retorno.intType);
        if (t == 2) {
            insertMAC(CRCT, stringConcat(intToString(t),  Tipo_Retorno.stringValue));
        } else {
            insertMAC(CRCT, stringConcat(intToString(t), Tipo_Retorno.stringValue));
        }
        break;
    case CCONS_STR:
        if (!isIOSentence) {
            error_handler(86);
        }

        insertMAC(CRCTS, intToString(dameCS()));
        unescapeNewline(sbol->lexema);
        unescapeTab(sbol->lexema);

        pushCS(sbol->lexema);

        Tipo_Retorno.expresionType= constIntToString;
        strcpy(Tipo_Retorno.stringValue, sbol->lexema);
        scanner();
        break;
    case CPAR_ABR:
        scanner();
        Tipo_Retorno= expresion(folset |  CPAR_CIE |  NADA);

        if (sbol->codigo == CPAR_CIE) {
            scanner();
        } else {
            error_handler(20);
        }

        break;
    case CNEG:
        scanner();
        Tipo_Retorno= expresion(folset);
        t= getTipo(Tipo_Retorno.intType);

        insertMAC(NEG, intToString(t));
        break;
    default:
        error_handler(31);
        Tipo_Retorno.intType= en_tabla("TIPOERROR");
    }

    test(folset, NADA | NADA, 69);

    return Tipo_Retorno;
}


struct TipoAttr variable(set folset) {
    char lexema[TAM_LEXEMA], t;
    int Iden_No_Declarado= 0;
    struct TipoAttr TipoE, Tipo_Retorno;


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
                    lista_expresiones(folset |  CPAR_CIE |  NADA);
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
                    inf_id->desc.part_var.arr.cant_elem= 99999;

                    scanner();
                    expresion(folset |  CCOR_CIE |  NADA);

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
                TipoE= expresion(folset |  CCOR_CIE |  NADA);

                if (TipoE.expresionType == constOfExpresionType) {
                    if (TipoE.intType == en_tabla("float") || TipoE.value < 0) {
                        error_handler(85);
                    } else if (TipoE.value >= ts[en_tabla(lexema)].ets->desc.part_var.arr.cant_elem) {
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
                    Tipo_Retorno.intType= ts[en_tabla(lexema)].ets->desc.part_var.arr.ptero_tipo_base;


                }
            } else if (Tipo_Ident(lexema) == en_tabla("TIPOARREGLO")&&!isFunctionInvocation) {
                error_handler(43);
            } else {

                Tipo_Retorno.intType= Tipo_Ident(lexema);


                t= getTipo(Tipo_Retorno.intType);

                insertMAC(CRVL, stringConcat(stringConcat(intToString(ts[en_tabla(lexema)].ets->desc.nivel), intToString(ts[en_tabla(lexema)].ets->desc.despl)), intToString(t)));

                if (Tipo_Retorno.intType == en_tabla("TIPOARREGLO")) {
                    Tipo_Retorno.baseType= ts[en_tabla(lexema)].ets->desc.part_var.arr.ptero_tipo_base;
                }
            }
        }

        Tipo_Retorno.expresionType= bunchOfVariables;
        Tipo_Retorno.despl= ts[en_tabla(lexema)].ets->desc.despl;
        Tipo_Retorno.level= ts[en_tabla(lexema)].ets->desc.nivel;
    } else {
        error_handler(16);
    }

    test(folset, NADA | NADA, 71);

    return Tipo_Retorno;
}


struct TipoAttr llamada_funcion(set folset) {
    char lexema[TAM_LEXEMA];
    struct TipoAttr Tipo_Retorno;
    isFunctionInvocation= 1;

    inInvocation= en_tabla(sbol->lexema);

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

    parameterQuantity= 0;

    if (sbol->codigo == CMAS 	  || sbol->codigo == CMENOS    ||
            sbol->codigo == CIDENT    || sbol->codigo == CPAR_ABR  ||
            sbol->codigo == CNEG      || sbol->codigo == CCONS_ENT ||
            sbol->codigo == CCONS_FLO || sbol->codigo == CCONS_CAR ||
            sbol->codigo == CCONS_STR) {
        lista_expresiones(folset |  CPAR_CIE |  NADA);
    }

    if (parameterQuantity != ts[en_tabla(lexema)].ets->desc.part_var.sub.cant_par) {
        error_handler(88);
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    isFunctionInvocation= 0;

    test(folset, NADA | NADA, 72);

    Tipo_Retorno.intType= Tipo_Ident(lexema);
    Tipo_Retorno.expresionType= function;

    return Tipo_Retorno;
}


void lista_expresiones(set folset) {
    struct TipoAttr TipoE;
    parameterQuantity= 0;


    TipoE= expresion(folset | CCOMA | NADA | F_EXPR);
    parameterQuantity++;


    paramChecking(TipoE, parameterQuantity);

    while (sbol->codigo == CCOMA || (sbol->codigo & F_EXPR)) {
        if ((sbol->codigo & F_EXPR)) {
            error_handler(75);
        } else {
            scanner();
        }

        TipoE= expresion(folset | CCOMA | NADA | F_EXPR);
        parameterQuantity++;

        paramChecking(TipoE, parameterQuantity);
    }
}

struct TipoAttr constante(set folset) {
    struct TipoAttr Tipo_Retorno;

    Tipo_Retorno.expresionType= constOfExpresionType;

    test(F_CONST,folset,73);
    switch (sbol->codigo) {
    case CCONS_ENT:


        Tipo_Retorno.value= atoi(sbol->lexema);
        strcpy(Tipo_Retorno.stringValue, sbol->lexema);

        constantQuantity++;
        scanner();
        Tipo_Retorno.intType= en_tabla("int");
        break;
    case CCONS_FLO:
        Tipo_Retorno.value= charToFloat(sbol->lexema);
        strcpy(Tipo_Retorno.stringValue, sbol->lexema);

        constantQuantity++;
        scanner();
        Tipo_Retorno.intType= en_tabla("float");
        break;
    case CCONS_CAR:
        Tipo_Retorno.value= sbol->lexema[1];
        strcpy(Tipo_Retorno.stringValue, intToString(Tipo_Retorno.value));

        constantQuantity++;
        scanner();
        Tipo_Retorno.intType= en_tabla("char");
        break;
    default:
        error_handler(38);

    }
    test(folset,NADA | NADA,74);
    return Tipo_Retorno;

}


