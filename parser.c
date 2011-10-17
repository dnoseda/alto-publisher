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
void constante();
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
void variable();
void expresion();
void expresion_asignacion();
void expresion_relacional();
void expresion_simple();
void relacion();
void termino();
void factor();
void llamada_funcion();
void lista_expresiones();


void scanner ();


/*************** variables auxiliares *************/
 
int punteroFuncion;  // donde apunto a la funcin donde estoy metido
int tipo_global;     // donde guardo en el caso     int a,b,c;    el tipo para b y c
int flag_hay_return; // dice si hay return en la declaracin de funcin
int void_flag;

/********** variables globales ************/


token *sbol;

//
int posID;
int posTabla;
int cantPar;
int tamARR=0;
char lexema[17];
int bandera;
int posicionTS;
int esParametro = 0;
tipo_inf_res *inicio; // puntero de la lista de parametros
tipo_inf_res *cursor; // cursor

//para ver la constante dentro de los corchetes.......

int esIndice = 0;


#define F_especificador_tipo (CVOID|CCHAR|CINT|CFLOAT)
#define F_constante (CCONS_CAR|CCONS_ENT|CCONS_FLO|CCONS_STR)
#define F_llamada_funcion (CIDENT)
#define F_variable (CIDENT)
#define F_relacion (CIGUAL|CDISTINTO|CMENOR|CMAYOR|CMEIG|CMAIG)
#define F_proposicion_e_s (CIN|COUT)
#define F_proposicion_seleccion (CIF)
#define F_proposicion_iteracion (CWHILE)
#define F_proposicion_retorno (CRETURN)
#define F_declarador_init (CASIGNAC|CCOR_ABR)
#define F_lista_declaraciones_init (CIDENT)
#define F_definicion_funcion (CPAR_ABR)
#define F_proposicion_compuesta (CLLA_ABR)

#define F_declaracion_params (F_especificador_tipo)
#define F_lista_declaracion_params (F_declaracion_params)
#define F_declaracion_variable (F_declarador_init|CCOMA|CPYCOMA)
#define F_lista_inicializadores (F_constante)


#define F_factor (F_variable|F_constante|CNEG|CPAR_ABR|F_llamada_funcion|CCONS_STR)
#define F_termino (F_factor)
#define F_expresion_simple (CMAS|CMENOS|F_termino)
#define F_expresion (F_expresion_simple)
#define F_lista_expresiones (F_expresion)
#define F_proposicion_expresion (F_expresion|CPYCOMA)
#define F_declaracion (F_especificador_tipo)
#define F_proposicion (F_proposicion_expresion|F_proposicion_compuesta|F_proposicion_seleccion|F_proposicion_iteracion|F_proposicion_e_s|F_proposicion_retorno)
#define F_lista_declaraciones (F_declaracion)
#define F_lista_proposiciones (F_proposicion)
#define F_unidad_traduccion (F_declaracion)
#define F_especificador_declaracion (F_definicion_funcion|F_declaracion_variable)

extern FILE *yyin;

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


set first(enum noTerminales not) {
    switch(not) {
    case declaracione:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case especificador_de_tip:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case especificador_de_declaracione:
        return cons(CPAR_ABR|CASIGNAC|CCOR_ABR|CCOMA|CPYCOMA, NADA);

    case definicion_de_funcio:
        return cons(CPAR_ABR ,NADA);

    case lista_declaracion_de_parametro:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case declaracion_de_parametr:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case declaracion_de_variabl:
        return cons(CASIGNAC|CCOR_ABR|CCOMA|CPYCOMA, NADA);

    case lista_declaraciones_ini:
        return cons(NADA,CIDENT);

    case declarador_ini:
        return cons(CASIGNAC|CCOR_ABR, NADA);

    case lista_de_inicializadore:
        return cons(NADA,CCONS_FLO|CCONS_CAR|CCONS_ENT);

    case proposicion_compuest:
        return cons(CLLA_ABR,NADA);

    case lista_de_declaracione:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case declaracio:
        return cons(CINT|CFLOAT|CVOID|CCHAR,NADA);

    case lista_de_proposicione:
        return cons(CWHILE|CIF|CIN|COUT|CLLA_ABR|CPAR_ABR,CMAS|CMENOS|CIDENT|CCONS_FLO|CCONS_CAR|CCONS_ENT|CNEG|CCONS_STR|CRETURN);

    case proposicio:
        return cons(CWHILE|CIF|CIN|COUT|CLLA_ABR|CPAR_ABR|CPYCOMA,CMAS|CMENOS|CIDENT|CCONS_FLO|CCONS_CAR|CCONS_ENT|CNEG|CCONS_STR|CRETURN);
        //first entrada salida no lo usamos
    case proposicion_de_iteracio:
        return cons(CWHILE,NADA);

    case proposicion_de_seleccio:
        return cons(CIF,NADA);

    case proposicion_de_retorn:
        return cons(NADA,CRETURN);

    case proposicion_expresio:
        return cons(CPAR_ABR|CPYCOMA, CMAS|CMENOS|CIDENT|CCONS_ENT|CCONS_FLO|CCONS_CAR|CNEG|CCONS_STR);

    case expresio:
        return cons(CPAR_ABR,CMAS|CMENOS|CIDENT|CCONS_ENT|CCONS_FLO|CCONS_CAR|CNEG|CCONS_STR);

    case relacio:
        return cons(NADA,CMAYOR|CMENOR|CIGUAL|CMEIG|CMAIG|CDISTINTO);

    case expresion_simpl:
        return cons(CPAR_ABR,CMAS|CMENOS|CIDENT|CCONS_ENT|CCONS_FLO|CCONS_CAR|CNEG|CCONS_STR);

    case termin:
        return cons(CPAR_ABR,CIDENT|CCONS_ENT|CCONS_FLO|CCONS_CAR|CNEG|CCONS_STR);

    case facto:
        return cons(CPAR_ABR,CIDENT|CCONS_ENT|CCONS_FLO|CCONS_CAR|CNEG|CCONS_STR);

    case variabl:
        return cons(NADA,CIDENT);

    case llamada_a_funcio:
        return cons(NADA,CIDENT);

    case lista_de_expresione:
        return cons(CPAR_ABR,CMAS|CMENOS|CCONS_ENT|CCONS_FLO|CCONS_CAR|CIDENT|CNEG|CCONS_STR);

    case constant:
        return cons(NADA,CCONS_ENT|CCONS_FLO|CCONS_CAR);

    default:
        return cons(NADA,NADA);

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

//metodos char to int
int chartoInt(char str[]) {
    int num;

    num = atoi(str);

    return num;
}//FIN metodos char to int


/********* funciones del parser ***********/

void unidad_traduccion(set folset) {
    test(F_unidad_traduccion | folset, NADA, 50);
    while (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT || sbol->codigo == CFLOAT) {
        declaraciones(folset | F_declaracion);
        test(F_declaracion,folset,51)
    }
}

void declaraciones(set folset) {
    
    especificador_tipo(folset | CIDENT | F_especificador_declaracion);
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }
    especificador_declaracion(folset);
}

void especificador_tipo(set folset) {

    test(F_especificador_tipo,folset,51);
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
    test(F_especificador_declaracion,folset,53);
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

    bandera=0;
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

        lista_declaraciones_param(folset | CPAR_CIE | first_proposicion_compuesta);

    } else {

        if(sbol->codigo == CIDENT) {
            lista_declaraciones_param(folset | CPAR_CIE | first_proposicion_compuesta);
        }
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion_compuesta(folset);

    if(posicionTS!=en_tabla("void")) {
        if(bandera!=1) {
            error_handler(37);
        }
    }

    pop_nivel();

}

void lista_declaraciones_param(set folset) {

    declaracion_parametro(CCOMA | folset | F_declaracion_params);

    while (sbol->codigo ==CCOMA || sbol->codigo & F_declaracion_params) {
        if(sbol->codigo & F_declaracion_params) {
            error_handler(75);
        } else {
            scanner();
        }
        declaracion_parametro(CCOMA | folset | F_declaracion_params);
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


    tipo_inf_res *elemento=NULL; // nuevo elemento

    if ((elemento = (tipo_inf_res *) malloc (sizeof (tipo_inf_res))) == NULL) {
        error_handler(41);
    } else {

        elemento->ptero_tipo = inf_id->ptr_tipo;
        elemento->tipo_pje=inf_id->desc.part_var.tipo_pje;
        if(inicio==NULL) {
            inicio=elemento;
            cursor=inicio;
        } else {
            cursor->ptr_sig=elemento;
            cursor=elemento;
        }
    }

    test(folset,NADA,55);

    insertarTS();

}

void lista_declaraciones_init(set folset) {

    test(F_lista_declaraciones_init, folset | CCOMA | F_declarador_init, 57);
    if (sbol->codigo == CIDENT) {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    } else {
        error_handler(16);
    }

    declarador_init(CCOMA  | CIDENT | F_declarador_init | folset);

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

        declarador_init(CCOMA  | CIDENT | F_declarador_init | folset);
    }

}

void declaracion_variable(set folset) {
    declarador_init(une(une(cons(CCOMA|CPYCOMA,NADA),first(lista_declaraciones_ini)),folset));

    if (sbol->codigo == CCOMA) {
        scanner();
        lista_declaraciones_init(une(folset,cons(CPYCOMA, NADA)));
    }
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
}



void declarador_init(set folset) {
    test(une(first(declarador_ini),folset),first(constant),58);
    char local[17];
//el declarador_init hace un push de una vble
    inf_id->ptr_tipo=posID;
    inf_id->cant_byte=ts[posID].ets->cant_byte; //en la TS en donde esta guardado el tipo le doy la cant de byte
    inf_id->clase=CLASVAR;




    if(in(sbol->codigo,first(constant))) {
        error_handler(79);
        constante(folset);
    } else {
        switch (sbol->codigo) {
        case CASIGNAC: { //es una variable y estoy viendo el signo igual
            scanner();
            constante(folset);//la constante que asigna
            break;
        }
        case CCOR_ABR: { //estoy declarando un arreglo
            scanner();
            if (sbol->codigo == CCONS_ENT) {

                strcpy(local,sbol->lexema);
                constante(une(une(folset,first(lista_de_inicializadore)),cons(CCOR_CIE|CASIGNAC,NADA)));//la constante que asigna
                //
                tamARR = chartoInt(local);//paso a entero
                inf_id->desc.part_var.arr.cant_elem = tamARR;
                //
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

                lista_inicializadores(une(folset,cons(CLLA_CIE,NADA)));

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


    test(folset,cons(NADA,NADA),59);

}

void lista_inicializadores(set folset) {
    constante(une(une(first(constant),folset),cons(CCOMA,NADA)));
    while (sbol->codigo == CCOMA||in(sbol->codigo,first(constant))) {
        if(in(sbol->codigo,first(constant))) {
            error_handler(75);
        } else {
            scanner();
        }
        tamARR++;
        constante(une(une(first(constant),folset),cons(CCOMA,NADA)));
    }
}


void proposicion_compuesta(set folset) {

    test(first(proposicion_compuest),une(une(une(first(lista_de_declaracione),first(lista_de_proposicione)),folset),cons(CLLA_CIE,NADA)),60);
    if (sbol->codigo == CLLA_ABR) {
        scanner();
    } else {
        error_handler(23);
    }

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        lista_declaraciones(une(folset,une(first(lista_de_proposicione),cons(CLLA_CIE,NADA))));
    }

    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN) {


        lista_proposiciones(une(folset,cons(CLLA_CIE,NADA)));
    }

    if (sbol->codigo == CLLA_CIE) {


        scanner();
    } else {
        error_handler(24);
    }
    test(folset,cons(NADA,NADA),61);

}
void lista_declaraciones(set folset) {

    declaracion(une(folset,first(declaracio)));

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

    {
        declaracion(une(folset,first(declaracio)));
    }

}

void declaracion(set folset) {

    especificador_tipo(une(une(folset,first(lista_declaraciones_ini)),cons(CPYCOMA,NADA)));

    lista_declaraciones_init(une(folset,cons(CPYCOMA,NADA)));

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }


    test(folset,cons(NADA,NADA),62);
}

void lista_proposiciones(set folset) {

    proposicion(une(folset,first(proposicio)));

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

    {
        proposicion(une(folset,first(proposicio)));
    }

}

void proposicion(set folset) {

    test(first(proposicio),folset,63);
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

    expresion(une(une(folset,first(proposicio)),cons(CPAR_CIE,NADA)));

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

    expresion(une(une(folset,first(proposicio)),cons(CPAR_CIE|CELSE,NADA)));

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    proposicion(une(une(cons(CELSE,NADA),folset),first(proposicio)));

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

        variable(une(une(folset,cons(CSHR|CPYCOMA,NADA)),first(variabl)));

        while (sbol->codigo == CSHR || in(sbol->codigo,first(variabl))) {
            if(in(sbol->codigo,first(variabl))) {
                error_handler(76);
            } else {
                scanner();
            }
            variable(une(une(folset,cons(CSHR|CPYCOMA,NADA)),first(variabl)));

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

        expresion(une(une(folset,cons(CSHL|CPYCOMA,NADA)),first(expresio)));

        while (sbol->codigo == CSHL || in(sbol->codigo,first(expresio))) {
            if(in(sbol->codigo,first(expresio))) {
                error_handler(77);
            } else {
                scanner();
            }
            expresion(une(une(folset,cons(CSHL|CPYCOMA,NADA)),first(expresio)));
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
    test(folset,cons(NADA,NADA),64);
}


void proposicion_retorno(set folset) {
    bandera = 1;
    scanner();
    expresion(une(folset,cons(CPYCOMA,NADA)));
    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    test(folset,cons(NADA,NADA),65);
}


void proposicion_expresion(set folset) {
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

    {
        expresion(une(folset,cons(CPYCOMA,NADA)));
    }

    if (sbol->codigo == CPYCOMA) {
        scanner();
    } else {
        error_handler(22);
    }
    test(folset,cons(NADA,NADA),66);
}


void expresion(set folset) {
    expresion_simple(une(une(folset,first(expresio)),cons(CASIGNAC, CDISTINTO|CIGUAL|CMENOR|CMEIG|CMAYOR|CMAIG)));

    if (in(sbol->codigo, first(expresio))) {
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

    test(first(expresion_simpl),une(une(first(termin),cons(NADA,COR)),folset),67);	//ver esteeeeeeeeeeeeeee
    if (sbol->codigo == CMAS || sbol->codigo == CMENOS) {
        scanner();
    }

    termino(une(une(folset,cons(NADA,CMAS|CMENOS|COR)),first(termin)));

    while ((sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR) || in(sbol->codigo,first(termin))) {
        if(in(sbol->codigo,first(termin))) {
            error_handler(78);
        } else {
            scanner();
        }
        termino(une(une(folset,cons(NADA,CMAS|CMENOS|COR)),first(termin)));
    }
}

void termino(set folset) {

    factor(une(une(folset,cons(NADA,CMULT|CDIV|CAND)),first(facto)));

    while ((sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)|| in(sbol->codigo,first(facto))) {
        if(in(sbol->codigo,first(facto))) {
            error_handler(78);
        } else {
            scanner();
        }
        factor(une(une(folset,cons(NADA,CMULT|CDIV|CAND)),first(facto)));
    }

}

void factor(set folset) {

    test(first(facto), folset, 68);
    switch (sbol->codigo) {
    case CIDENT: {
        if (Clase_Ident(sbol->lexema)== CLASFUNC) {
            inicio = ts[en_tabla(sbol->lexema)].ets->desc.part_var.sub.ptr_inf_res;
            cantPar=ts[en_tabla(sbol->lexema)].ets->desc.part_var.sub.cant_par;
            esParametro = 1;
            llamada_funcion(folset);
            inicio=NULL;
            cursor=NULL;
            esParametro = 0;
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
        expresion(une(folset,cons(CPAR_CIE,NADA)));
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

    test(folset,cons(NADA,NADA), 69);
}

void variable(set folset) {
    char lexema[17];
    int pos;

    test(first(variabl),folset,70);

    if (sbol->codigo == CIDENT) {
        strcpy(lexema,sbol->lexema);
        scanner();
    } else {

        error_handler(16);


    }
    if (sbol->codigo == CCOR_ABR) {

        scanner();
        //printf("sbol->lexema antes %s\n",sbol->lexema);
        expresion(une(folset,cons(CPAR_CIE|CCOR_CIE,NADA)));
        //printf("sbol->lexema %s\n",sbol->lexema);
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
            printf("naaaaaaaaaaa\n");
        }
    } else { // si no es un corchete que abre
        if((en_tabla(lexema)==NIL)&&(sbol->codigo!=CCONS_ENT)) { //comparo que no sea constante... por que si es trata de buscar el identificador
            //printf("naaaaaaaaaa     %s\n",sbol->lexema) ;
            strcpy(inf_id->nbre,lexema);
            inf_id->clase= CLASVAR;
            posID=en_tabla("TIPOERROR");
            inf_id->ptr_tipo = posID;
            inf_id->cant_byte = ts[posID].ets->cant_byte;
            insertarTS();
            error_handler(33);
        } else { //si esta en la tabla
            if((esParametro==0) && (Tipo_Ident(lexema)==en_tabla("TIPOARREGLO"))) {
                error_handler(40);
            }
            /*if(Tipo_Ident(lexema)==en_tabla("TIPOARREGLO")){

            	error_handler(40);
                }*/
        }
    }


    test(folset,cons(NADA,NADA),71);

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
        lista_expresiones(une(cons(CPAR_CIE,NADA),folset));
    }

    if (sbol->codigo == CPAR_CIE) {
        scanner();
    } else {
        error_handler(20);
    }

    test(folset,cons(NADA,NADA),72);

}

void lista_expresiones(set folset) {
    expresion(une(une(folset,cons(CCOMA,NADA)),first(expresio)));
    while ((sbol->codigo == CCOMA)|| in(sbol->codigo,first(expresio))) {
        if(in(sbol->codigo,first(expresio))) {
            error_handler(75);
        } else {
            scanner();
        }
        expresion(une(une(folset,cons(CCOMA,NADA)),first(expresio)));
    }
}
void constante(set folset) {

    test(first(constant),folset,73);
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

// printf("sbol->lexema en constanteeeeeeeeeeeeeee %s\n",sbol->lexema); //no encuentra  ] entra a lest hace un sacenner y encuentra ;
    test(folset,cons(NADA,NADA),74);
    //test(une(folset,cons(CCOR_CIE,NADA)),cons(NADA,NADA),74);
    //printf("sbol->lexema en constanteeeeeeeeeeeeeee %s\n",sbol->lexema);
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











