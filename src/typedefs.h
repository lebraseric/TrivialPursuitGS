/*
 *  Trivial Pursuit GS
 *
 *  Types definitions
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <quickdraw.h>
#include <types.h>

/*  Macros  */

#define dtItemListLength 0x11

#define LNOM            16
#define DMAX            9
#define PARTS           10
#define DE              11
#define MAXDEST         7
#define LANCER_DE       0
#define CHOISIR_CASE    1
#define VERSION         0x0100

typedef struct {
    GSString255 chemin;
    char nom[15];
    Boolean nouveau, action;
} Jeu;

typedef struct {
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
} PJeu;

typedef struct {
    Byte pixelMap[32000];
    AnSCBByte SCBByte[200];
    Byte filler[56];
    ColorTable tableCoul[16];
} Ecran;

typedef struct {
   Word qCarte, qSujet, qDiff;
   char qQuestion[120], qReponse[120];
   char category[30];
   char id[25];
   char incorrectAnswer[3][120];
   char type[30];
   char difficulty[10];
} tQuestion;

typedef struct {
   char nom[30];
   Word version;
   Word nbQuest;
   char theme[6][30];
} tInfoRec;

typedef struct {
   Word length;
   char data[];
} DataBlock;

#endif TYPEDEFS_H
