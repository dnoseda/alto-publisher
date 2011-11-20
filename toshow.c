
char *getStringINST(int INST)
{
    char *sINST= (char *)calloc (1, 13);
    strcpy(sINST, ">>ERROR<<");
    switch(INST)
    {
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

