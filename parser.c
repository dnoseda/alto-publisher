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

enum typeExpresion {variables,
                    unaVariable,
                    Constant,
                    vars_consts,
                    funcion,
                    Const_iToStr
                   };


struct Tipo {
    enum    typeExpresion typeExpresionresion;
    int    tipo;
    int    tipo_base;
    float   valor;
    char    sValor[150];
    int     nivel;
    int     despl;
};


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

int main( int argc,char *argv[]) {

    inic_tablas();
    linea = (char *) malloc (2);
    strcat(linea, "");
    nro_linea=0;
    if (argc != 3) {
        error_handler(6);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    } else {
        if(strcmp(argv[1],"-c")!=0) {
            error_handler(0);
            error_handler(COD_IMP_ERRORES);
            exit(1);
        } else {
            if ((yyin = fopen(argv[2], "r" )) == NULL) {
                error_handler(7);
                error_handler(COD_IMP_ERRORES);
                exit(1);
            }
        }
    }
    sbol=&token1;
    scanner();
    unidad_traduccion(CEOF);

    if(Clase_Ident("main")!=CLASFUNC) {
        error_handler(15);
        error_handler(COD_IMP_ERRORES);

    } else {
        if(ts[en_tabla("main")].ets->desc.part_var.sub.cant_par!=0) {
            error_handler(36);
            error_handler(COD_IMP_ERRORES);
        }
        if(Tipo_Ident("main")!= en_tabla("void")) {
            error_handler(35);
            error_handler(COD_IMP_ERRORES);
        }
    }
    if (sbol->codigo != CEOF) {
        error_handler(8);
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

    bandera=FALSE;
    inf_id->clase=CLASFUNC;
    inf_id->ptr_tipo=posID;
    inf_id->cant_byte=ts[posID].ets->cant_byte;
    posTabla=insertarTS();
    posicionTS = posID;
    pushTB();

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT || sbol->codigo == CFLOAT) {

        lista_declaraciones_param(folset | CPAR_CIE | F_PROP_COMP);

    } else {

        if(sbol->codigo == CIDENT) {
            lista_declaraciones_param(folset | CPAR_CIE | F_PROP_COMP);
        }
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion_compuesta(folset);

    if(posicionTS!=en_tabla("void")) {
        if(bandera!=TRUE) {
            error_handler(37);
        }
    }

    pop_nivel();

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

void declaracion_parametro(set folset) {

    especificador_tipo(folset | CCOR_ABR | CAMPER | CCOR_CIE | CIDENT);

    cantPar++;
    inf_id->ptr_tipo=posID;
    inf_id->clase=CLASPAR;
    inf_id->cant_byte=ts[posID].ets->cant_byte;



    if (sbol->codigo == CAMPER) {
        scanner();
        inf_id->desc.part_var.tipo_pje='r';

    } else {
        inf_id->desc.part_var.tipo_pje='v';
    }

    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }

    if (sbol->codigo == CCOR_ABR) {


        inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
        inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
        inf_id->cant_byte = (ts[posID].ets->cant_byte);

        scanner();
        if (sbol->codigo == CCOR_CIE) {
            scanner();
        } else {
            error_handler(21);
        }

    }


    tipo_inf_res *aux_inf_res=NULL;

    if ((aux_inf_res = (tipo_inf_res *) malloc (sizeof (tipo_inf_res))) == NULL) {
        error_handler(41);
    } else {

        aux_inf_res->ptero_tipo = inf_id->ptr_tipo;
        aux_inf_res->tipo_pje=inf_id->desc.part_var.tipo_pje;
        if(inicio==NULL) {
            inicio=aux_inf_res;
            cursor=inicio;
        } else {
            cursor->ptr_sig=aux_inf_res;
            cursor=aux_inf_res;
        }
    }

    test(folset,NADA,55);

    insertarTS();

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
    test(F_DECL_INIT | folset,F_CONST,58);
    char local[17];

    inf_id->ptr_tipo=posID;
    inf_id->cant_byte=ts[posID].ets->cant_byte;
    inf_id->clase=CLASVAR;

    if(sbol->codigo & F_CONST) {
        error_handler(79);
        constante(folset);
    } else {
        switch (sbol->codigo) {
        case CASIGNAC: {
            scanner();
            constante(folset);
            break;
        }
        case CCOR_ABR: {
            scanner();
            if (sbol->codigo == CCONS_ENT) {

                strcpy(local,sbol->lexema);
                constante(folset | F_LIST_INIC | CCOR_CIE | CASIGNAC,NADA);

                tamARR = atoi(local);
                inf_id->desc.part_var.arr.cant_elem = tamARR;

            }
            if (sbol->codigo == CCOR_CIE) {
                scanner();
            } else {
                error_handler(21);
            }
            if (sbol->codigo == CASIGNAC) {
                scanner();
                if (sbol->codigo == CLLA_ABR) {
                    scanner();
                } else {
                    error_handler(23);
                }

                tamARR=0;

                lista_inicializadores(folset | CLLA_CIE );

                inf_id->desc.part_var.arr.cant_elem = tamARR;
                if (sbol->codigo == CLLA_CIE) {
                    scanner();
                } else {
                    error_handler(24);
                }
            }
            inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
            inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
            inf_id->cant_byte = ((inf_id->cant_byte)*tamARR);

            break;
        }
        }

    }
    if(!(inf_id->nbre[0]=='\0')) {
        insertarTS();
    }


    test(folset,NADA,59);

}

void lista_inicializadores(set folset) {
    constante( F_CONST | folset | CCOMA);
    while (sbol->codigo == CCOMA || sbol->codigo & F_CONST) {
        if(sbol->codigo & F_CONST) {
            error_handler(75);
        } else {
            scanner();
        }
        tamARR++;
        constante(F_CONST | folset | CCOMA);
    }
}


void proposicion_compuesta(set folset) {

    test(F_PROP_COMP,( F_LIST_DECL | F_LIST_PROP|folset|CLLA_CIE ),60);
    if (sbol->codigo == CLLA_ABR) {
        scanner();
    } else {
        error_handler(23);
    }

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        lista_declaraciones(folset | F_LIST_PROP |CLLA_CIE,NADA);
    }

    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN) {


        lista_proposiciones(folset | CLLA_CIE);
    }

    if (sbol->codigo == CLLA_CIE) {


        scanner();
    } else {
        error_handler(24);
    }
    test(folset,NADA,61);

}
void lista_declaraciones(set folset) {

    declaracion(folset|F_DECL);

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        declaracion(folset|F_DECL);
    }

}

void declaracion(set folset) {

    especificador_tipo(folset|F_LIST_DECL_INIT|CPYCOMA);

    lista_declaraciones_init(folset|CPYCOMA);

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }


    test(folset,NADA,62);
}

void lista_proposiciones(set folset) {
    proposicion(folset|F_PROP);

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

    {
        proposicion(folset|F_PROP);
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

    expresion(folset|F_PROP|CPAR_CIE);

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion(folset);
}


void proposicion_seleccion(set folset) {

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

    expresion(folset|F_PROP|CPAR_CIE|CELSE);

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion(CELSE|folset|F_PROP);

    if (sbol->codigo == CELSE) {
        scanner();
        proposicion(folset);
    }
}

void proposicion_e_s(set folset) {
    switch(sbol->codigo) {
    case CIN: {
        scanner();
        if (sbol->codigo == CSHR) {
            scanner();
        } else {
            error_handler(28);
        }

        variable(folset|CSHR|CPYCOMA|F_VAR);

        while (sbol->codigo == CSHR || sbol->codigo & F_VAR) {
            if(sbol->codigo & F_VAR) {
                error_handler(76);
            } else {
                scanner();
            }
            variable(folset|CSHR|CPYCOMA|F_VAR);

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

        expresion(folset|CSHL|CPYCOMA|F_EXPR);

        while (sbol->codigo == CSHL || sbol->codigo & F_EXPR) {
            if(sbol->codigo & F_EXPR) {
                error_handler(77);
            } else {
                scanner();
            }
            expresion(folset|CSHL|CPYCOMA|F_EXPR);
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
    test(folset,NADA,64);
}


void proposicion_retorno(set folset) {
    bandera = TRUE;
    scanner();
    expresion(folset|CPYCOMA);
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    test(folset,NADA,65);
}


void proposicion_expresion(set folset) {
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

    {
        expresion(folset|CPYCOMA);
    }

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    test(folset,NADA,66);
}


void expresion(set folset) {
    expresion_simple(folset|F_EXPR|CASIGNAC| CDISTINTO|CIGUAL|CMENOR|CMEIG|CMAYOR|CMAIG);

    if (sbol->codigo &  F_EXPR) {
        error_handler(78);
        expresion(folset);
    } else {
        switch (sbol->codigo) {
        case CASIGNAC: {
            scanner();
            expresion(folset);
            break;
        }
        case CDISTINTO:
        case CIGUAL:
        case CMENOR:
        case CMEIG:
        case CMAYOR:
        case CMAIG: {
            scanner();
            expresion(folset);
            break;
        }
        }
    }
}

void expresion_simple(set folset) {

    test(F_EXP_SIMPLE,F_TERM|COR|folset,67);
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
        scanner();
    }

    termino(folset|CMAS|CMENOS|COR|F_TERM);

    while ((sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR) || sbol->codigo & F_TERM) {
        if(sbol->codigo & F_TERM) {
            error_handler(78);
        } else {
            scanner();
        }
        termino(folset | CMAS|CMENOS|COR| F_TERM);
    }
}

void termino(set folset) {

    factor(folset| CMULT|CDIV|CAND | F_FACTOR);

    while ((sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)|| sbol->codigo & F_FACTOR) {
        if(sbol->codigo & F_FACTOR) {
            error_handler(78);
        } else {
            scanner();
        }
        factor(folset| CMULT|CDIV|CAND | F_FACTOR);
    }

}

void factor(set folset) {

    test(F_FACTOR, folset, 68);
    switch (sbol->codigo) {
    case CIDENT: {
        if (Clase_Ident(sbol->lexema)== CLASFUNC) {
            inicio = ts[en_tabla(sbol->lexema)].ets->desc.part_var.sub.ptr_inf_res;
            cantPar=ts[en_tabla(sbol->lexema)].ets->desc.part_var.sub.cant_par;
            esParametro = TRUE;
            llamada_funcion(folset);
            inicio=NULL;
            cursor=NULL;
            esParametro = FALSE;
        } else {
            variable(folset);
        }

        break;
    }
    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
        constante(folset);
        break;
    case CCONS_STR:
        scanner();
        break;
    case CPAR_ABR: {
        scanner();
        expresion(folset|CPAR_CIE);
        if (sbol->codigo == CPAR_CIE) {
            scanner();
        } else {
            error_handler(20);
        }
        break;
    }
    case CNEG: {
        scanner();
        expresion(folset);
        break;
    }
    default:
        error_handler(31);
    }

    test(folset,NADA, 69);
}

void variable(set folset) {
    char lexema[17];
    int pos;

    test(F_VAR,folset,70);

    if (sbol->codigo == CIDENT) {
        strcpy(lexema,sbol->lexema);
        scanner();
    } else {

        error_handler(16);


    }
    if (sbol->codigo == CCOR_ABR) {

        scanner();

        expresion(folset|CPAR_CIE|CCOR_CIE);

        if (sbol->codigo == CCOR_CIE) {
            if(en_tabla(lexema)==NIL) {
                strcpy(inf_id->nbre,lexema);
                inf_id->clase= CLASVAR;
                posID=en_tabla("TIPOERROR");
                inf_id->ptr_tipo = posID;
                inf_id->cant_byte = ts[posID].ets->cant_byte;
                inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
                inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
                inf_id->cant_byte = ts[posID].ets->cant_byte;
                insertarTS();
                error_handler(33);
            } else if(Tipo_Ident(lexema)!=en_tabla("TIPOARREGLO")) {
                error_handler(32);
            }
            scanner();
        } else {
            error_handler(21);
        }
    } else {
        if((en_tabla(lexema)==NIL)&&(sbol->codigo!=CCONS_ENT)) {

            strcpy(inf_id->nbre,lexema);
            inf_id->clase= CLASVAR;
            posID=en_tabla("TIPOERROR");
            inf_id->ptr_tipo = posID;
            inf_id->cant_byte = ts[posID].ets->cant_byte;
            insertarTS();
            error_handler(33);
        } else {
            if((esParametro==FALSE) && (Tipo_Ident(lexema)==en_tabla("TIPOARREGLO"))) {
                error_handler(40);
            }
        }
    }


    test(folset,NADA,71);

}
void llamada_funcion(set folset) {

    if (sbol->codigo == CIDENT) {
        scanner();
    } else {
        error_handler(16);
    }

    if (sbol->codigo == CPAR_ABR) {
        scanner();
    } else {
        error_handler(19);
    }

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

    {
        lista_expresiones(CPAR_CIE|folset);
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    test(folset,NADA,72);

}

void lista_expresiones(set folset) {
    expresion(folset|CCOMA|F_EXPR);
    while ((sbol->codigo == CCOMA)|| sbol->codigo & F_EXPR) {
        if(sbol->codigo & F_EXPR) {
            error_handler(75);
        } else {
            scanner();
        }
        expresion(folset|CCOMA|F_EXPR);
    }
}
void constante(set folset) {

    test(F_CONST,folset,73);
    switch (sbol->codigo) {
    case CCONS_ENT:
        scanner();
        break;
    case CCONS_FLO:
        scanner();
        break;
    case CCONS_CAR:
        scanner();
        break;
    default: {
        error_handler(38);
    }

    }

    test(folset,NADA,74);
}

