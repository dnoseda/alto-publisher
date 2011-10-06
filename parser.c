/******************************************************************
  v5.3         Este es el archivo correspondiente al PARSER
 *******************************************************************/

#define EXTERNA extern

#include <stdlib.h>
#include <stdio.h>
#include "codigos.h"
#include "var_globales.h"
#include "ts.c"

#define TRUE (1 == 1)
#define FALSE (!TRUE)

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

int punteroFuncion;  // donde apunto a la funciòn donde estoy metido
int tipo_global;     // donde guardo en el caso     int a,b,c;    el tipo para b y c
int flag_hay_return; // dice si hay return en la declaraciòn de funciòn
int void_flag;

/****************************/

/*************************** check_main() *******************************/
void check_main()
{
    if (en_tabla("main") != NIL)
    {
        if (en_tabla("void") != Tipo_Ident("main"))
            error_handler(35); // main debe ser void/*&*/

        if(ts[en_tabla("main")].ets->desc.part_var.sub.cant_par != 0)
            error_handler(36);// funcion main no lleva argumentos/*&*/

    }
    else
        error_handler(15);// falta funcion main/*&*/
    error_handler(COD_IMP_ERRORES);

}
/*************************** check_main() *******************************/

/********** variables globales ************/



token *sbol;

extern FILE *yyin;

int inInvocation = FALSE;

void scanner()
{
    int i;

    for (; (i=yylex())!= NADA && sbol->codigo == SEGUIR;);
    if (i == NADA) sbol->codigo = CEOF;
    /* yylex retorna 0 si llego a fin de archivo */

    liberar = linea;
    linea = (char *) malloc (strlen(linea) + strlen (token1.lexema) + 3);
    strcpy(linea, liberar);
    strcat(linea, token1.lexema);
    free ( (void *) liberar);

    /* El alumno debera dar una solucion al problema que las constantes
       enteras y flotantes son entregadas como strings al parser */
}


int main( int argc,char *argv[])
{

    /* el alumno debera inicializar la variable yyin segun corresponda */

    linea = (char *) malloc (2);
    strcat(linea, "");

    nro_linea=0;

    // inicializo la tabla de simbolos
    inic_tablas();
    // inicializo lista parametros
    init_lista_param();

    if (argc != 3)
    {
        error_handler(6);
        error_handler(COD_IMP_ERRORES);
        exit(1);
    }
    else
    {
        if(strcmp(argv[1],"-c") == 0)
        {
            if ((yyin = fopen(argv[2], "r" )) == NULL)
            {
                error_handler(7);
                error_handler(COD_IMP_ERRORES);
                exit(1);
            }
        }
        else
            printf("falta el -c\n");
    }

    sbol=&token1 ;/* la variable token */

    scanner();
    unidad_traduccion();
    check_main();
    //

    if (sbol->codigo != CEOF)
        error_handler(8);

}


/********* funciones del parser ***********/

void unidad_traduccion()
{

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        declaraciones();
}

void declaraciones()
{

    especificador_tipo();
    if (sbol->codigo == CIDENT)
    {
        strcpy(inf_id->nbre,sbol->lexema); //especifica el nombre del identificador
        scanner();
    }
    else
        error_handler(16);    // falta identificador

    especificador_declaracion();
}

void especificador_tipo()
{

    switch (sbol->codigo)   // especifica el tipo en inf_id
    {
    case CVOID:
        inf_id->ptr_tipo = en_tabla("void");
        scanner();
        break;
    case CCHAR:
        inf_id->ptr_tipo = en_tabla("char");
        scanner();
        break;
    case CINT:
        inf_id->ptr_tipo = en_tabla("int");
        scanner();
        break;
    case CFLOAT:
        inf_id->ptr_tipo = en_tabla("float");
        scanner();
        break;
    default:
        error_handler(17); // tipo no definido
    }
}

void especificador_declaracion()
{

    switch (sbol->codigo)
    {
    case CPAR_ABR:
        definicion_funcion();
        break;
    case CASIGNAC:   // %&
    case CCOR_ABR:   // tipo arreglo
    case CCOMA:
    case CPYCOMA:
        declaracion_variable();
        break;
    default:
        error_handler(18); // error en especificar declaracin
    }

}

void definicion_funcion()
{

    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19); // falta parentesis que abre

    inf_id->clase = CLASFUNC;
    void_flag = (inf_id->ptr_tipo == en_tabla("void"));
    if (!(punteroFuncion = insertarTS()))
        error_handler(9);  // guardo el puntero a esta funciòn /*IsTs*/
    pushTB();  // nuevo bloque

    if (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)
        lista_declaraciones_param();

    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20); // falta parentesis que cierra

    // ligamos lista de par�etros con funcin de turno
    ts[punteroFuncion].ets->desc.part_var.sub.cant_par = lista_param.cant;
    ts[punteroFuncion].ets->desc.part_var.sub.ptr_inf_res = lista_param.base;
    init_lista_param();

    flag_hay_return = FALSE;

    proposicion_compuesta();
    if (!void_flag && !flag_hay_return)
    {
        error_handler(37);  // falta return
    }

}

void lista_declaraciones_param()
{

    declaracion_parametro();

    while (sbol->codigo ==CCOMA)
    {
        scanner();

        declaracion_parametro();
    }
}

void declaracion_parametro()
{

    especificador_tipo();

    if (sbol->codigo == CAMPER)
    {
        scanner();
        nodo_a_ins->tipo_pje = REF;
    }
    else
        nodo_a_ins->tipo_pje = VALOR;

    nodo_a_ins->ptero_tipo = inf_id->ptr_tipo;



    if (sbol->codigo == CIDENT)
    {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    }
    else
        error_handler(16);    // falta identificador /&



    if (sbol->codigo == CCOR_ABR)  // es un arreglo
    {

        scanner();

        if (sbol->codigo == CCOR_CIE)
            scanner();
        else
            error_handler(21); // falta corchete que cierra

        inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo;
        inf_id->ptr_tipo = en_tabla("TIPOARREGLO");
        nodo_a_ins->tipo_pje = REF;
        // Control para colocar el tipo base correspondiente.
        if (nodo_a_ins->ptero_tipo == en_tabla("char"))
            nodo_a_ins->ptero_tipo = en_tabla("ARRCHAR");
        else if (nodo_a_ins->ptero_tipo == en_tabla("int")) nodo_a_ins->ptero_tipo = en_tabla("ARRINT");
        else if (nodo_a_ins->ptero_tipo == en_tabla("float")) nodo_a_ins->ptero_tipo = en_tabla("ARRFLOAT");
    }
    inf_id->clase = CLASPAR;
    inf_id->desc.part_var.tipo_pje = nodo_a_ins->tipo_pje;
    insertar_lista_param();
    if (!insertarTS())
        error_handler(9);
}

void lista_declaraciones_init()
{

    if (sbol->codigo == CIDENT)
    {
        strcpy(inf_id->nbre,sbol->lexema);
        scanner();
    }
    else
        error_handler(16);// falta identificador
    tipo_global = inf_id->ptr_tipo;
    inf_id->clase = CLASVAR;

    declarador_init();
    if (!insertarTS())
        error_handler(9);

    while (sbol->codigo == CCOMA)
    {

        scanner();

        if (sbol->codigo == CIDENT)
        {
            strcpy(inf_id->nbre,sbol->lexema);
            inf_id->ptr_tipo = tipo_global;
            inf_id->clase = CLASVAR;
            scanner();
        }
        else
            error_handler(16);// falta identificador

        declarador_init();
        if (!insertarTS())
            error_handler(9);
    }

}


void declaracion_variable()
{


    inf_id->clase = CLASVAR;

    declarador_init();

    if (sbol->codigo == CCOMA)
    {
        scanner();
        if(inf_id->ptr_tipo == en_tabla("TIPOARREGLO"))
            tipo_global = inf_id->desc.part_var.arr.ptero_tipo_base;
        else
            tipo_global = inf_id->ptr_tipo;
        if (!insertarTS())
            error_handler(9);
        lista_declaraciones_init();
    }
    else
    {
        if(strlen(inf_id->nbre) > 0 &&
                !insertarTS())
        {
            error_handler(9);
        }
    }

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22); // falta ;

}


void declarador_init()
{

    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        constante();
        break;
    }
    case CCOR_ABR:
    {
        inf_id->desc.part_var.arr.ptero_tipo_base = inf_id->ptr_tipo; //si es un arreglo paso el tipo al tipo base
        inf_id->ptr_tipo = en_tabla("TIPOARREGLO"); 		  // y el tipo es ahora TIPOARREGLO
        scanner();

        if (sbol->codigo == CCONS_ENT)
            constante();
        /** /
          else
          error_handler(41); // la cantidad de elementos debe estar especificada como constante entera
        /**/

        if (sbol->codigo == CCOR_CIE)
            scanner();
        else
            error_handler(21); // falta ]



        if (sbol->codigo == CASIGNAC)
        {
            scanner();

            if (sbol->codigo == CLLA_ABR)
                scanner();
            else
                error_handler(23); // falta llave que abre

            lista_inicializadores();

            if (sbol->codigo == CLLA_CIE)
                scanner();
            else
                error_handler(24); // falta llave que cierra

        }

        break;
    }
    }
}

void lista_inicializadores()
{

    constante();

    while (sbol->codigo == CCOMA)
    {
        scanner();

        constante();
    }

}


void proposicion_compuesta()
{

    if (sbol->codigo == CLLA_ABR)
        scanner();
    else
        error_handler(23); // falta llave que abre


    if (sbol->codigo == CVOID || sbol->codigo == CCHAR || sbol->codigo == CINT || sbol->codigo == CFLOAT)

        lista_declaraciones();

    if (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        lista_proposiciones();


    if (sbol->codigo == CLLA_CIE)
        scanner();
    else
        error_handler(24); // falta llave que cierra

    //imprimir_ts();
    pop_nivel();

}
void lista_declaraciones()
{

    declaracion();

    while (sbol->codigo == CVOID || sbol->codigo == CCHAR ||
            sbol->codigo == CINT || sbol->codigo == CFLOAT)

        declaracion();

}

void declaracion()
{

    especificador_tipo();

    lista_declaraciones_init();

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22); // falta ;

}

void lista_proposiciones()
{

    proposicion();

    while (sbol->codigo == CLLA_ABR || sbol->codigo == CMAS ||
            sbol->codigo == CMENOS || sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR ||
            sbol->codigo == CIF || sbol->codigo == CWHILE ||
            sbol->codigo == CIN || sbol->codigo == COUT ||
            sbol->codigo == CPYCOMA || sbol->codigo == CRETURN)

        proposicion();

}

void proposicion()
{

    switch (sbol->codigo)
    {
    case CLLA_ABR:
        proposicion_compuesta();
        break;
    case CWHILE:
        proposicion_iteracion();
        break;
    case CIF:
        proposicion_seleccion();
        break;
    case CIN:
    case COUT:
        proposicion_e_s();
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
        proposicion_expresion();
        break;
    case CRETURN:
        proposicion_retorno();
        break;
    default:
        error_handler(25); // error en proposicin
    }
}

void proposicion_iteracion()
{

    if (sbol->codigo == CWHILE)
        scanner();
    else
        error_handler(26); // falta while

    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19); // falta parentesis que abre

    expresion();

    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20); // falta parentesis que cierra
    pushTB();
    proposicion();

}


void proposicion_seleccion()
{

    if (sbol->codigo == CIF)
        scanner();
    else
        error_handler(27); // falta if

    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19); // falta parentesis que abre

    expresion();

    if (sbol->codigo == CPAR_CIE)
        scanner();
    else
        error_handler(20); // falta parentesis que cierra
    pushTB();
    proposicion();

    if (sbol->codigo == CELSE)
    {
        scanner();
        pushTB();
        proposicion();
    }

}

void proposicion_e_s()
{

    switch(sbol->codigo)
    {
    case CIN:
    {
        scanner();
        if (sbol->codigo == CSHR)
            scanner();
        else
            error_handler(28); // falta  >>
        variable();
        while (sbol->codigo == CSHR)
        {
            scanner();
            variable();
        }
        if (sbol->codigo == CPYCOMA)
            scanner();
        else
            error_handler(22); // falta ;
        break;
    }
    case COUT:
    {
        scanner();
        if (sbol->codigo == CSHL)
            scanner();
        else
            error_handler(29); // falta <<
        expresion();
        while (sbol->codigo == CSHL)
        {
            scanner();
            expresion();
        }
        if (sbol->codigo == CPYCOMA)
            scanner();
        else
            error_handler(22); // falta ;
        break;
    }
    default:
        error_handler(30); // %& //falta << o >>
    }
}


void proposicion_retorno()
{

    flag_hay_return = TRUE;
    scanner();
    expresion();
    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22); // falta ;

}


void proposicion_expresion()
{

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        expresion();

    if (sbol->codigo == CPYCOMA)
        scanner();
    else
        error_handler(22); // falta ;
}


void expresion()
{

    expresion_simple();

    switch (sbol->codigo)
    {
    case CASIGNAC:
    {
        scanner();
        expresion();
        break;
    }
    case CDISTINTO:
    case CIGUAL:
    case CMENOR:
    case CMEIG:
    case CMAYOR:
    case CMAIG:
    {
        scanner();
        expresion();
        break;
    }
    }
}


void expresion_simple()
{

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS)
        scanner();

    termino();

    while (sbol->codigo == CMAS || sbol->codigo == CMENOS || sbol->codigo == COR)
    {
        scanner();
        termino();
    }

}

void termino()
{

    factor();

    while (sbol->codigo == CMULT || sbol->codigo == CDIV || sbol->codigo == CAND)
    {
        scanner();
        factor();
    }

}

void factor()
{
    int clase_rerere_loca;
    switch (sbol->codigo)
    {
    case CIDENT:

        if(en_tabla(sbol->lexema) != NIL)/*&*/
        {
            clase_rerere_loca = Clase_Ident(sbol->lexema);
            switch (clase_rerere_loca)
            {
            case CLASPAR:
            case CLASVAR:
                variable();
                break;
            case CLASFUNC:
                llamada_funcion();
                break;
            default:
                error_handler(8);// palabra reservada
            }
        }
        else
        {
            // no esta en la tabla
            error_handler(33); // identificador no declarado
            inf_id->ptr_tipo = en_tabla("TIPOERROR");
            variable();
        }

        break;

    case CCONS_ENT:
    case CCONS_FLO:
    case CCONS_CAR:
        constante();
        break;
    case CCONS_STR:
        scanner();
        break;
    case CPAR_ABR:
    {
        scanner();
        expresion();
        if (sbol->codigo == CPAR_CIE)
            scanner();
        else
            error_handler(20);// falta )
        break;
    }
    case CNEG:
    {
        scanner();
        expresion();
        break;
    }
    default:
        error_handler(31); // error de factor
    }

}

void variable()
{
    char str_aux[9]; /*&*/
    if (sbol->codigo == CIDENT)
    {
        if(inf_id->ptr_tipo == en_tabla("TIPOERROR")) {/*&*/ // si no esta declarada me interesa
            strcpy(inf_id->nbre,sbol->lexema); 	   // el nombre del id para insertarlo
            inf_id->clase=CLASVAR;
        }
        strcpy(str_aux,sbol->lexema);/*&*/ //necesito saber el nbre del ident para chekear si es tipo arreglo
        scanner();
    }
    else error_handler(16); // falta identificador



    if (sbol->codigo == CCOR_ABR)
    {
        if(inf_id->ptr_tipo == en_tabla("TIPOERROR")){/*&*/ // si es un arreglo no declarado me interesa guardarlo
            inf_id->ptr_tipo = en_tabla("TIPOARREGLO"); // como arreglo
            inf_id->desc.part_var.arr.ptero_tipo_base = en_tabla("TIPOERROR");
        }
        else
        {
            if(Tipo_Ident(str_aux) != en_tabla("TIPOARREGLO")) // si esta en la tabla me fijo que sea arreglo
                error_handler(32); // la variable no es de tipo arreglo
        }
        scanner();
        expresion();
        if (sbol->codigo == CCOR_CIE)
            scanner();
        else
            error_handler(21); // falta ]
    }
    else
    {
        if(inf_id->ptr_tipo != en_tabla("TIPOERROR") && Tipo_Ident(str_aux) == en_tabla("TIPOARREGLO")
                && !inInvocation) // checkeo no estar en una invocacion
        {
            error_handler(40); // en una expresiòn las variables deben ser accedidas por sus elementos
        }
    }
    if(inf_id->ptr_tipo == en_tabla("TIPOERROR"))
    {
        if (!insertarTS())
        {
            error_handler(9);
        }
    }
    else
    {
        if(inf_id->ptr_tipo == en_tabla("TIPOARREGLO") && (inf_id->desc.part_var.arr.ptero_tipo_base == en_tabla("TIPOERROR")))
        {
            if (!insertarTS())
            {
                error_handler(9);
            }
        }
    }

}


void llamada_funcion()
{

    if (sbol->codigo == CIDENT)
        scanner();
    else
        error_handler(34); // se esperaba identificador de funcion
    inInvocation  = TRUE;
    if (sbol->codigo == CPAR_ABR)
        scanner();
    else
        error_handler(19); // falta (

    if (sbol->codigo == CMAS || sbol->codigo == CMENOS ||
            sbol->codigo == CIDENT ||
            sbol->codigo == CPAR_ABR || sbol->codigo == CNEG ||
            sbol->codigo == CCONS_ENT || sbol->codigo == CCONS_FLO ||
            sbol->codigo == CCONS_CAR || sbol->codigo == CCONS_STR)

        lista_expresiones();

    if (sbol->codigo == CPAR_CIE)
    {
        scanner();
    }
    else
    {
        error_handler(20); // falta )
    }
    inInvocation  = FALSE;
}

void lista_expresiones()
{

    expresion();

    while (sbol->codigo == CCOMA)
    {
        scanner();

        expresion();
    }

}

void constante()
{

    switch (sbol->codigo)
    {
    case CCONS_ENT:
        if (inf_id->ptr_tipo == en_tabla("TIPOARREGLO"))
            inf_id->desc.part_var.arr.cant_elem = atoi(sbol->lexema);

        scanner();
        break;
    case CCONS_FLO:
        scanner();
        break;
    case CCONS_CAR:
        scanner();
        break;
    default:
        error_handler(38);
        scanner(); /*f_error(); aca va f_error, faltan los algoritmos de conversion a las constantes numericas. */
    }

}















