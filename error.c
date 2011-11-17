#define EXTERNA extern

#include <stdio.h>
#include "error.h"
#include "var_globales.h"

char error =0;

void error_print (int ne) {
    error = 1;
    //printf("\t Error %d: *****************************************ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ\n", ne);
    switch (ne) {
        /* errores de scanner */
    case 1:
        printf("\t Error %d: Fin de comentario no encontrado\n", ne);
        break;
    case 2:
        printf("\t Error %d: El String es demasiado largo\n", ne);
        break;
    case 3:
        printf("\t Warning %d: falta comilla de cierre. la línea será consumida\n", ne);
        break;
    case 4:
        printf("\t Warning %d: Identificador de mas de 8 caracteres, será truncado\n", ne);
        break;
    case 5:
        printf("\t Error %d: Caracter invalido\n", ne);
        break;
    case 6:
        printf("\t Error %d: El número de argumentos no coincide. ucc programa.c \n", ne);
        break;
    case 7:
        printf("\t Error %d: El archivo no existe\n ", ne);
        break;
    case 8:
        printf("\t Error %d: Ocurrió un error \n", ne);
        break;
        /* errores de la tabla de simbolos*/
    case 9:
        printf("\t Error %d: Identificador ya declarado\n", ne);
        break;
    case 10:
        printf("\t Error %d: Falta Memoria\n", ne);
        break;
    case 11:
        printf("\t Error %d: Tabla de Símbolos vacía\n", ne);
        break;
    case 12:
        printf("\t Error %d: Tabla de Símbolos llena\n", ne);
        break;
    case 13:
        printf("\t Error %d: Tabla de Bloques llena\n", ne);
        break;
    case 14:
        printf("\t Error %d: Tabla de Bloques vacía\n", ne);
        break;
        /* errores de parser*/
    case 15:
        printf("\t Error %d: Falta declarar la funcion main\n", ne);
        break;
    case 16:
        printf("\t Error %d: Falta identificador\n", ne);
        break;
    case 17:
        printf("\t Error %d: Tipo no definido \n", ne);
        break;
    case 18:
        printf("\t Error %d: En Especificar Declaración\n", ne);
        break;
    case 19:
        printf("\t Error %d: Falta (\n", ne);
        break;
    case 20:
        printf("\t Error %d: Falta )\n", ne);
        break;
    case 21:
        printf("\t Error %d: Falta ]\n", ne);
        break;
    case 22:
        printf("\t Error %d: Falta ;\n", ne);
        break;
    case 23:
        printf("\t Error %d: Falta {\n", ne);
        break;
    case 24:
        printf("\t Error %d: Falta }\n", ne);
        break;
    case 25:
        printf("\t Error %d: Error en proposición\n", ne);
        break;
    case 26:
        printf("\t Error %d: Falta While\n", ne);
        break;
    case 27:
        printf("\t Error %d: Falta if\n", ne);
        break;
    case 28:
        printf("\t Error %d: Falta >>\n", ne);
        break;
    case 29:
        printf("\t Error %d: Falta <<\n", ne);
        break;
    case 30:
        printf("\t Error %d: Falta << o >>\n", ne);
        break;
    case 31:
        printf("\t Error %d: En factor\n", ne);
        break;
    case 32:
        printf("\t Error %d: La variable no es de tipo arreglo \n", ne);
        break;
    case 33:
        printf("\t Error %d: Identificador no declarado\n", ne);
        break;
    case 34:
        printf("\t Error %d: Se esperaba identificador de función\n", ne);
        break;
    case 35:
        printf("\t Error %d: El tipo de la funcion main() debe ser void\n", ne);
        break;
    case 36:
        printf("\t Error %d: La funcion main() no lleva argumento\n", ne);
        break;
    case 37:
        printf("\t Error %d: Falta return\n", ne);
        break;
    case 38:
        printf("\t Error %d: Falta constante\n", ne);
        break;
    case 39:
        printf("\t Error %d: El identificador debe ser una variable o parámetro\n", ne);
        break;
    case 40:
        printf("\t Error %d: En una expresion, los arreglos deben ser accedidos por sus elementos\n", ne);
        break;
    case 41:
        printf("\t Error %d: No hoy mas memoria....\n", ne);
        break;
    case 43:
        printf("\t Error %d: La variable es de tipo Arreglo, se esperan conchetes\n", ne);
        break;


        /* errores de recuperacion de errores */
    case 50:
        printf("\t Error %d: Al inicio del programa. \n", ne);
        break;
    case 51:
        printf("\t Error %d: Símbolo inesperado o falta especificador de tipo. \n", ne);
        break;
    case 52:
        printf("\t Error %d: Símbolo inesperado después de especificador de tipo. \n", ne);
        break;
    case 53:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de especif. de declaración \n", ne);
        break;
    case 54:
        printf("\t Error %d: Símbolo inesperado después de especificador de declaración \n", ne);
        break;
    case 55:
        printf("\t Error %d: Símbolo inesperado después de declarar un parámetro. \n", ne);
        break;
    case 56:
        printf("\t Error %d: Símbolo inesperado después de declarar un parámetro \n", ne);
        break;
    case 57:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de lista decl. init.\n", ne);
        break;
    case 58:
        printf("\t Error %d: Símbolo inesperado al comienzo de declarador init. \n", ne);
        break;
    case 59:
        printf("\t Error %d: Símbolo inesperado después de declarador init. \n", ne);
        break;
    case 60:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de proposición compuesta. \n", ne);
        break;
    case 61:
        printf("\t Error %d: Símbolo inesperado después de proposición compuesta. \n", ne);
        break;
    case 62:
        printf("\t Error %d: Símbolo inesperado después de declaración. \n", ne);
        break;
    case 63:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de proposición. \n", ne);
        break;
    case 64:
        printf("\t Error %d: Símbolo inesperado después de proposición entrada - salida. \n", ne);
        break;
    case 65:
        printf("\t Error %d: Símbolo inesperado después de proposición retorno. \n", ne);
        break;
    case 66:
        printf("\t Error %d: Símbolo inesperado después de proposición expresión. \n", ne);
        break;
    case 67:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de expresión simple. \n", ne);
        break;
    case 68:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de factor. \n", ne);
        break;
    case 69:
        printf("\t Error %d: Símbolo inesperado después de factor. \n", ne);
        break;
    case 70:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de variable. \n", ne);
        break;
    case 71:
        printf("\t Error %d: Símbolo inesperado después de variable. \n", ne);
        break;
    case 72:
        printf("\t Error %d: Símbolo inesperado después de llamada a función. \n", ne);
        break;
    case 73:
        printf("\t Error %d: Símbolo inesperado o falta símb. al comienzo de constante\n", ne);
        break;
    case 74:
        printf("\t Error %d: Símbolo inesperado despues de constante,\n", ne);
        break;
    case 75:
        printf("\t Error %d: Falta , \n", ne);
        break;
    case 76:
        printf("\t Error %d: Falta >> \n", ne);
        break;
    case 77:
        printf("\t Error %d: Falta << \n", ne);
        break;
    case 78:
        printf("\t Error %d: Falta *, /, &&, +, -, ||, = u operador de relación \n", ne);
        break;
    case 79:
        printf("\t Error %d: Falta = \n", ne);
        break;
    case 80:
        printf("\t Error %d:  \n", ne);
        break;
    case 81:
        printf("\t Error %d: Un arreglo no puede estar vacion, debe tener al menos un elemento \n", ne);
        break;
    case 82:
        printf("\t Error %d: Una variable o un parametro no puede ser de tipo VOID \n", ne);
        break;
    case 83:
        printf("\t Error %d: La cardinalidad del arreglo se especifica con un tipo entero \n", ne);
        break;
    case 84:
        printf("\t Error %d: A la izquierda de una asignacion debe ir una variable \n", ne);
        break;
    case 85:
        printf("\t Error %d: Indices no validos del arreglo \n", ne);
        break;
    case 86:
        printf("\t Error %d: Un String solo puede estar en una proposicion de E/S \n", ne);
        break;
    case 87:
        printf("\t Error %d: Indice Negativo\n", ne);
        break;

    case 88:
        printf("\t Error %d: La cantidad de parametros en la invocacion debe conincidir con la cantida de la declaracion\n", ne);
        break;
    case 89:
        printf("\t Error %d: No se puede pasar un arreglo por direccion\n", ne);
        break;
    case 90:
        printf("\t Error %d: Tipos de parametros desiguales\n", ne);
        break;
    case 91:
        printf("\t Error %d: En el parámetro real sólo debe aparecer un identificador.\n", ne);
        break;
    case 92:
        printf("\t Error %d: El parámetro real debe ser una variable.\n", ne);
        break;
    case 93:
        printf("\t Error %d: Asignacion no valida.\n", ne);
        break;
    case 94:
        printf("\t Error %d: La cantidad de elementos en la lista de inicializacion no coincide con el tamaño del arreglo.\n", ne);
        break;













    }
}


void error_handler(int ne) {
    int i;

    if (ne == COD_IMP_ERRORES) {
        printf("Linea %d - %s\n", nro_linea, linea);
        for (i=0; i < cant_errores_x_linea; i++) {
            error_print( errores_x_linea [i]);
        }
        cant_errores_x_linea=0;
    } else {
        errores_x_linea [cant_errores_x_linea] = ne;
        cant_errores_x_linea++;
    }
}





/*


void f_error() {
   printf(" %d.- Ocurrio un error\n", linea);
}
*/
