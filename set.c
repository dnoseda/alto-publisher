/****************************************************************************

		       	C O M P I L A D O R E S


Modelo para la implementacion de tipo set en 'C'.

En este fuente, se ve en primer termino, la definicion de las constantes
que representan los tokens devueltos por el scanner(); luego las funciones
que implementan operaciones sobre conjuntos de acuerdo a esas definiciones

****************************************************************************/


# include <stdio.h>
# include "codigos.h"
# include "set.h"


/****************************************************************************
						une()
****************************************************************************/
set une(set c1, set c2) {
    set u;

    u.p1=c1.p1|c2.p1;
    u.p2=c1.p2|c2.p2;
    return u;
}
/****************************************************************************
						in()
****************************************************************************/
int in(long e,set A) { /* x pertenece a A ? */
    long r;

    if ((e & (UNO<<30)) != UNO<<30 ) {
        r= (e & A.p1);
    } else {
        r=(e & A.p2);
    }
    if (e == r) {
        return 1;
    } else {
        return 0;
    }

}

/****************************************************************************
						cons()
****************************************************************************/
set cons(long x,long y) {
    set nuevo_set;

    nuevo_set.p1 = x;
    nuevo_set.p2 = y;
    return nuevo_set;
}
/***********************************************************************
                            imprime set
***********************************************************************/
void imp_set(set v) {
    printf("%X\n",v.p1);
    printf("%X \n",v.p2);
};


/*
int main () {
  int result;

  long l1, l2, e;
  set s1;
  set s2;

  l2= 0x40000030;
  l1= 0x00000001;

  s1=cons(l1, l2);

  e=0x40000002;
  result= in(e, s1);

  imp_set(s1);

  printf("esta %X en el set ? %d\n",e,result);
}

*/















