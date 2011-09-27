// v_1



tipo_inf_res *nodo_a_ins;

struct zzz{
	tipo_inf_res *base;
   tipo_inf_res *ultimo;
   int cant;
}lista_param;



void init_lista_param()
{
   lista_param.base = NULL;
   lista_param.ultimo = NULL;
   lista_param.cant=0;

   if ( (nodo_a_ins = (tipo_inf_res*)malloc(sizeof(tipo_inf_res))) == NULL)
   {
   	perror("init_lista_param");
      exit(1);
   }
   nodo_a_ins->ptr_sig = NULL;

}

int insertar_lista_param()
{
	if(lista_param.base == NULL)
   {
   	lista_param.base = nodo_a_ins;
      lista_param.ultimo = nodo_a_ins;
   }
   else
   {
   	lista_param.ultimo->ptr_sig = nodo_a_ins;
      lista_param.ultimo = lista_param.ultimo->ptr_sig;
   }

   if ( (nodo_a_ins = (tipo_inf_res*)malloc(sizeof(tipo_inf_res))) == NULL)
   {
   	perror("init_lista_param");
      exit(1);
   }
   nodo_a_ins->ptr_sig = NULL;

   lista_param.cant++;
}

