
char *getFriendlyString(int INST)
{
    char* tabla[1000];

    tabla[ALM] = sALM  ;
    tabla[ALMI] = sALMI ;
    tabla[ALOC] = sALOC ;
    tabla[AND] = sAND  ;
    tabla[BIFF] = sBIFF ;
    tabla[BIFS] = sBIFS ;
    tabla[CAST] = sCAST ;
    tabla[CHPR] = sCHPR ;
    tabla[CMAI] = sCMAI ;
    tabla[CMEI] = sCMEI ;
    tabla[CMIG] = sCMIG ;
    tabla[CMMA] = sCMMA ;
    tabla[CMME] = sCMME ;
    tabla[CMNI] = sCMNI ;
    tabla[CRCT] = sCRCT ;
    tabla[CRCTS] = sCRCTS;
    tabla[CRDI] = sCRDI ;
    tabla[CRVL] = sCRVL ;
    tabla[CRVLI] = sCRVLI;
    tabla[DIV] = sDIV  ;
    tabla[DMEM] = sDMEM ;
    tabla[ENBL] = sENBL ;
    tabla[ENPR] = sENPR ;
    tabla[FINB] = sFINB ;
    tabla[IMPCS] = sIMPCS;
    tabla[IMPR] = sIMPR ;
    tabla[INPP] = sINPP ;
    tabla[INV] = sINV  ;
    tabla[LEER] = sLEER ;
    tabla[MUL] = sMUL  ;
    tabla[NEG] = sNEG  ;
    tabla[OR] = sOR   ;
    tabla[PARAR] = sPARAR;
    tabla[POP] = sPOP  ;
    tabla[RTPR] = sRTPR ;
    tabla[SUB] = sSUB  ;
    tabla[SUM] = sSUM  ;

    char *sINST= (char *)calloc (1, 13);
    strcpy(sINST, ">>ERROR<<");
    if(tabla[INST] > 0){
        strcpy(sINST,tabla[INST]);
    }

    return sINST;
}

