/*
 *  os.1 -- Acces fichiers
 */


/*  Header application  */

#include "trivial.h"


/*  Headers C standards  */

#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>


/*  Headers Toolbox  */

#include <types.h>
#include <window.h>
#include <gsos.h>


/*  Donnees de main.1  */

extern struct {
    GSString255 chemin;
    char nom[15];
    Boolean nouveau, action;
} jeu;
extern struct {
    Word diff;
    Word de;
    Word tour;
    Word etape;
    Boolean gagnee;
    Byte dest[MAXDEST];
    int ndest;
    struct {
        char nom[LNOM];
        Word camemberts;
        Byte position;
        Boolean enJeu;
        struct {
            Word essai;
            Word succes;
        } total[6];
    } joueur[6];
} pJeu;


/*  Variables statiques  */

#define private static

private FILE *fbase;
private char *p;
private word t, n, nmax;
private Boolean baseOuverte = FALSE;
private struct _index {
   char typeQuest;
   word carte;
   long adrQuest;
} *index;
OpenRec openDCB = { 15, 0, &jeu.chemin };
IORec IODCB = { 5 };
RefNumRec closeDCB = { 1 };


int initBase(cheminBase, infoRec)
char *cheminBase;
tInfoRec *infoRec;
{
   word l;

   if (!(fbase = fopen(cheminBase, "rb"))) return errno;
   if (!fread((char *)&l, sizeof(l), 1, fbase)) return errno;
   if (!fread((char *)infoRec, sizeof(tInfoRec), 1, fbase)) return errno;
   nmax = n = infoRec->nbQuest - 1;
   fseek(fbase, (long)(l + 2), 0);
   if (!fread((char *)&l, sizeof(l), 1, fbase)) return errno;
   if (!(index = (struct _index *)malloc(l))) return -1;
   if (!fread((char *)index, sizeof(struct _index),nmax,fbase)) return errno;
   baseOuverte = TRUE;
   return 0;
}

void reInitBase()
{
   word i;

   for (i=0; i<nmax; index[i++].typeQuest &= 0x7f);
}

int nouvQuest(question)
tQuestion *question;
{
   word i, j, d, rc;

   d = question->qDiff <= 3 ? question->qDiff : 3;
   if (n) {
       i = rand() % nmax;
       for (j = 0; ((word)(index[i].typeQuest & 0x07) != question->qSujet
                   || (word)(index[i].typeQuest >> 3 & 0x07) > d
                   || index[i].typeQuest & 0x80
                   || question->qDiff == 4
                        && (word)(index[i].typeQuest >> 3 & 0x07) != 3)
                   && j < nmax; j++)
       {
           i++;
           if (i == nmax) i = 0;
       }
       if (j < nmax) {
           question->qCarte = index[i].carte;
           question->qDiff = (word)(index[i].typeQuest >> 3 & 0x07);
           rc = decode(question, index[i].adrQuest, fbase);
           index[i].typeQuest |= 0x80;
           n--;
       }
       else
           rc = -3;
   }
   else
       rc = -2;
   return rc;
}

int fermeBase()
{
    if (baseOuverte) {
        fclose(fbase);
        free(index);
        baseOuverte = FALSE;
        return 0;
    }
}

int decode(question, loc, fic)
tQuestion *question;
long loc;
FILE *fic;
{
   word l;
   char buffer[256];

   fseek(fic, loc, 0);
   if (!fread((char *)&l, sizeof(l), 1, fic)) return errno;
   if (!fread(buffer, 1, l, fic)) return errno;
   p = buffer; t=0;
   litchamps(question->qQuestion);
   litchamps(question->qReponse);
   return 0;
}

void litchamps(dest)
register char *dest;
{
   while (*dest++ = litc());
}

char litc()
{
    static char table[3][32] = {
        {0,0,' ',0x88,0x8e,0x8f,'a','b','c','d','e','f','g','h','i','j','k','l',
        'm','n','o','p','q','r','s','t','u','v','w','x','y','z'},
        {'\0','!','"',0x89,0x9d,'%','&','\'','(',')',0x99,'+',',','-','.',0x8d,
        '0','1','2','3','4','5','6','7','8','9',':',';','<','=',0xa1,'?'},
        {0x90,0x91,0x94,0x95,0x9e,0x9f,'A','B','C','D','E','F','G','H','I','J','K',
        'L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}
    };
    char c;

    switch (c = nextc()) {
        case 0 : c = table[1][nextc()];
                 break;
        case 1 : c = table[2][nextc()];
                 break;
        default : c = table[0][c];
    }
    return c;
}

char nextc()
{
   static union {
       word utw;
       char utc[2];
   } ut;

   if (t)
       t--;
   else {
       ut.utc[1] = *p++;
       ut.utc[0] = *p++;
       t = 2;
   }
   return (char)(ut.utw >> (5 * t + 1) & 0x001f);
}

void Ouvrir()
{
    Word version;
    char verStr[8];
    Pointer toVerStr = verStr;

    Open(&openDCB);
    IODCB.refNum = closeDCB.refNum = openDCB.refNum;
    IODCB.dataBuffer = (Pointer)&version;
    IODCB.requestCount = (LongWord)sizeof(version);
    Read(&IODCB);
    if (version <= VERSION) {
        IODCB.dataBuffer = (Pointer)&pJeu;
        IODCB.requestCount = (LongWord)sizeof(pJeu);
        Read(&IODCB);
    } else {
        sprintf(verStr, "%.1d.%.1d", version / 256, version & 0x00ff);
        AlertWindow(0, &toVerStr,
                "33/Jeu sauvegard\216 avec Trivial Pursuit version *0./^#6");
    }
    Close(&closeDCB);
}

void Enregistrer()
{
    static CreateRec createDCB = { 7, &jeu.chemin, 0xc3, 0xF6, 0x6367};
    static Word version = VERSION;

    Create(&createDCB);
    Open(&openDCB);
    IODCB.refNum = closeDCB.refNum = openDCB.refNum;
    IODCB.dataBuffer = (Pointer)&version;
    IODCB.requestCount = (LongWord)sizeof(version);
    Write(&IODCB);
    IODCB.dataBuffer = (Pointer)&pJeu;
    IODCB.requestCount = (LongWord)sizeof(pJeu);
    Write(&IODCB);
    Close(&closeDCB);
    jeu.action = FALSE;
}
