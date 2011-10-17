
/**************************************************************/
////////////////// PROTOTIPOS //////////////////////////////////
/**************************************************************/

void imprimir_tipo(int b,int t);
void imprimir_lista_param(tipo_inf_res *ptr, int t);
void imprimir_stc(stc_TS p, int clas, int t);
void imprimir_entrada(entrada_TS *p,int t);

/**************************************************************/
////////////////// IMPLEMENTACIONES //////////////////////////// v_1
/**************************************************************/

void tab(int t) { // imprime una cantidad t de tabs, o sea da margen
    for(; t>0; t--) {
        printf("\t");
    }
}

/**
  -----------------------  imprimir_lista_param  -------------------------
  @param inf_res *ptr
  @param int t
  */
void imprimir_lista_param(tipo_inf_res *p, int t) { // imprime una lista de parametros
    int i;
    tab(t);
    printf("Lista de parametros:\n");
    if(p == NULL) {
        tab(t);
        printf("Sin par�etros.\n");
    } else {
        i = 1;
        while(p!=NULL) {
            tab(t);
            printf("parametro nro: %d\n",i);
            tab(t);
            printf("tipo de pasaje: %c\n", p->tipo_pje);
            imprimir_tipo(p->ptero_tipo,t+1);
            p = p->ptr_sig;
            i++;
        }
    }
}



void imprimir_stc(stc_TS p, int clas, int t) { // imprime un descriptor
    tab(t);
    printf("stc_TS:\n");
    tab(t);
    printf("nivel lexicogr�ico: %d\n",p.nivel);
    tab(t);
    printf("desplazamiento: %d\n",p.despl);
    switch(clas) {
    case CLASVAR:
        tab(t);
        printf("arreglo:\n");
        tab(t);
        printf("tipo de elementos:\n");
        imprimir_tipo(p.part_var.arr.ptero_tipo_base,t+1);
        tab(t);
        printf("cantidad de elementos: %d\n",p.part_var.arr.cant_elem);
        break;
    case CLASFUNC:
        tab(t);
        printf("funcin:\n");
        tab(t);
        printf("direccin del cdigo: %d\n",p.part_var.sub.dir_cod);
        tab(t);
        printf("cantidad de par�etros: %d\n",p.part_var.sub.cant_par);
        imprimir_lista_param(p.part_var.sub.ptr_inf_res,t+1);
        break;
    case CLASPAR:
        tab(t);
        printf("Par�etro:\n");
        tab(t);
        printf("tipo de pasaje: %C\n",p.part_var.tipo_pje);
    }
}

/*************************************************************************/
/*---------------------- IMPRIMIR_ENTRADA -------------------------------*/
/*************************************************************************/
void imprimir_entrada(entrada_TS *p,int t) { // imprime una entrada
    if(p!=NULL) {
        tab(t);
        printf("nombre de identificador: %s\n",p->nbre);
        tab(t);
        printf("clase computacional: ");
        switch(p->clase) {
        case CLASTYPE:
            printf("CLASTYPE\n");
            tab(t);
            printf("cantidad de bytes: %d\n",p->cant_byte);
            break;
        case CLASVAR:
            printf("CLASVAR\n");
            imprimir_tipo(p->ptr_tipo,t+1);
            tab(t);
            printf("cantidad de bytes: %d\n",p->cant_byte);
            imprimir_stc(p->desc,p->clase,t+1);
            break;
        case CLASFUNC:
            printf("CLASFUNC\n");
            imprimir_tipo(p->ptr_tipo,t+1);
            tab(t);
            printf("cantidad de bytes: %d\n",p->cant_byte);
            imprimir_stc(p->desc,p->clase,t+1);
            break;
        case CLASPAR:
            printf("CLASPAR\n");
            imprimir_tipo(p->ptr_tipo,t+1);
            tab(t);
            printf("cantidad de bytes: %d\n",p->cant_byte);
            imprimir_stc(p->desc,p->clase,t+1);
            break;
        }

    }
}

/*************************************************************************/
/*------------------------- IMPRIMIR_TIPO -------------------------------*/
/*************************************************************************/

void imprimir_tipo(int b,int t) { // imprime un tipo
    tab(t);
    printf("Tipo:\n");
    imprimir_entrada(ts[b].ets,t+1);
}

/*************************************************************************/
/*------------------------- IMPRIMIR_TS -------------------------------*/
/*************************************************************************/

void imprimir_ts() {
    int i;
    for(i = 0; i<TAM_TS; i++)
        if(ts[i].ets != NULL) { // && ts[i].ets->clase != CLASTYPE)
            printf("ts[%d]:\n",i);
            printf("entrada_TS:\n");
            imprimir_entrada(ts[i].ets,1);
        }
}


/*************************************************************************/
/*------------------------- MAIN ----------------------------------------*/
/*************************************************************************/

main() {
    inic_tablas();
    // inserto una funcin
    strcpy(inf_id->nbre,"imprimir_tipo");
    inf_id->clase = CLASFUNC;
    inf_id->ptr_tipo = en_tabla("void");
    inf_id->desc.nivel = 1;
    inf_id->desc.despl = 99;
    inf_id->desc.part_var.sub.dir_cod = 0x889A;
    inf_id->desc.part_var.sub.cant_par = 2;
    inf_id->desc.part_var.sub.ptr_inf_res = (tipo_inf_res *) malloc(sizeof(tipo_inf_res));
    inf_id->desc.part_var.sub.ptr_inf_res->tipo_pje = 'v';
    inf_id->desc.part_var.sub.ptr_inf_res->ptero_tipo = en_tabla("int");
    inf_id->desc.part_var.sub.ptr_inf_res->ptr_sig = (tipo_inf_res *) malloc(sizeof(tipo_inf_res));
    inf_id->desc.part_var.sub.ptr_inf_res->ptr_sig->tipo_pje = 'v';
    inf_id->desc.part_var.sub.ptr_inf_res->ptr_sig->ptero_tipo = en_tabla("int");
    inf_id->desc.part_var.sub.ptr_inf_res->ptr_sig->ptr_sig = NULL;
    insertarTS();
    imprimir_ts();
    return 0;
}
