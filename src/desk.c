/*
 *  desk.1 -- Gestion des evenements desktop
 */


/*  Header application  */

#include "trivial.h"
#include "sounds.h"
#include "desk.h"
#include "dialogs.h"
#include "jfen.h"


/*  Headers Toolbox  */

#include <types.h>
#include <memory.h>
#include <quickdraw.h>
#include <event.h>
#include <window.h>
#include <dialog.h>
#include <qdaux.h>
#include <menu.h>
#include <stdlib.h>


static struct {
    Byte type;
    Word x, y;
    Byte adj[6];
} cases[] = {
    { 0x26, 111, 108, {  1, 13, 25, 37, 49, 61 }},  /*  0 */
    { 0x04, 110,  85, {  0,  2, -1, -1, -1, -1 }},  /*  1 */
    { 0x00, 110,  74, {  1,  3, -1, -1, -1, -1 }},  /*  2 */
    { 0x05, 110,  62, {  2,  4, -1, -1, -1, -1 }},  /*  3 */
    { 0x01, 110,  49, {  3,  5, -1, -1, -1, -1 }},  /*  4 */
    { 0x02, 110,  38, {  4,  6, -1, -1, -1, -1 }},  /*  5 */
    { 0x13, 110,  25, {  5,  7, 72, -1, -1, -1 }},  /*  6 */
    { 0x02, 127,  26, {  6,  8, -1, -1, -1, -1 }},  /*  7 */
    { 0x37, 140,  29, {  7,  9, -1, -1, -1, -1 }},  /*  8 */
    { 0x05, 154,  33, {  8, 10, -1, -1, -1, -1 }},  /*  9 */
    { 0x04, 166,  39, {  9, 11, -1, -1, -1, -1 }},  /* 10 */
    { 0x37, 175,  45, { 10, 12, -1, -1, -1, -1 }},  /* 11 */
    { 0x01, 185,  54, { 11, 18, -1, -1, -1, -1 }},  /* 12 */
    { 0x03, 134,  97, {  0, 14, -1, -1, -1, -1 }},  /* 13 */
    { 0x05, 145,  91, { 13, 15, -1, -1, -1, -1 }},  /* 14 */
    { 0x02, 157,  85, { 14, 16, -1, -1, -1, -1 }},  /* 15 */
    { 0x04, 168,  80, { 15, 17, -1, -1, -1, -1 }},  /* 16 */
    { 0x01, 182,  75, { 16, 18, -1, -1, -1, -1 }},  /* 17 */
    { 0x10, 197,  68, { 17, 19, 12, -1, -1, -1 }},  /* 18 */
    { 0x01, 205,  83, { 18, 20, -1, -1, -1, -1 }},  /* 19 */
    { 0x37, 207,  92, { 19, 21, -1, -1, -1, -1 }},  /* 20 */
    { 0x02, 209, 102, { 20, 22, -1, -1, -1, -1 }},  /* 21 */
    { 0x03, 209, 113, { 21, 23, -1, -1, -1, -1 }},  /* 22 */
    { 0x37, 207, 122, { 22, 24, -1, -1, -1, -1 }},  /* 23 */
    { 0x04, 204, 133, { 23, 30, -1, -1, -1, -1 }},  /* 24 */
    { 0x00, 133, 119, {  0, 26, -1, -1, -1, -1 }},  /* 25 */
    { 0x02, 145, 124, { 25, 27, -1, -1, -1, -1 }},  /* 26 */
    { 0x01, 157, 130, { 26, 28, -1, -1, -1, -1 }},  /* 27 */
    { 0x03, 170, 136, { 27, 29, -1, -1, -1, -1 }},  /* 28 */
    { 0x04, 181, 141, { 28, 30, -1, -1, -1, -1 }},  /* 29 */
    { 0x15, 196, 149, { 29, 31, 24, -1, -1, -1 }},  /* 30 */
    { 0x04, 184, 163, { 30, 32, -1, -1, -1, -1 }},  /* 31 */
    { 0x37, 174, 171, { 31, 33, -1, -1, -1, -1 }},  /* 32 */
    { 0x01, 164, 177, { 32, 34, -1, -1, -1, -1 }},  /* 33 */
    { 0x00, 153, 183, { 33, 35, -1, -1, -1, -1 }},  /* 34 */
    { 0x37, 140, 187, { 34, 36, -1, -1, -1, -1 }},  /* 35 */
    { 0x03, 128, 189, { 35, 42, -1, -1, -1, -1 }},  /* 36 */
    { 0x05, 110, 130, {  0, 38, -1, -1, -1, -1 }},  /* 37 */
    { 0x01, 110, 142, { 37, 39, -1, -1, -1, -1 }},  /* 38 */
    { 0x04, 110, 154, { 38, 40, -1, -1, -1, -1 }},  /* 39 */
    { 0x00, 110, 166, { 39, 41, -1, -1, -1, -1 }},  /* 40 */
    { 0x03, 110, 178, { 40, 42, -1, -1, -1, -1 }},  /* 41 */
    { 0x12, 110, 191, { 41, 43, 36, -1, -1, -1 }},  /* 42 */
    { 0x03,  92, 190, { 42, 44, -1, -1, -1, -1 }},  /* 43 */
    { 0x37,  79, 188, { 43, 45, -1, -1, -1, -1 }},  /* 44 */
    { 0x04,  68, 183, { 44, 46, -1, -1, -1, -1 }},  /* 45 */
    { 0x05,  56, 177, { 45, 47, -1, -1, -1, -1 }},  /* 46 */
    { 0x37,  45, 171, { 46, 48, -1, -1, -1, -1 }},  /* 47 */
    { 0x00,  36, 164, { 47, 54, -1, -1, -1, -1 }},  /* 48 */
    { 0x02,  87, 118, {  0, 50, -1, -1, -1, -1 }},  /* 49 */
    { 0x04,  75, 124, { 49, 51, -1, -1, -1, -1 }},  /* 50 */
    { 0x03,  63, 130, { 50, 52, -1, -1, -1, -1 }},  /* 51 */
    { 0x05,  51, 135, { 51, 53, -1, -1, -1, -1 }},  /* 52 */
    { 0x00,  39, 141, { 52, 54, -1, -1, -1, -1 }},  /* 53 */
    { 0x11,  25, 149, { 53, 55, 48, -1, -1, -1 }},  /* 54 */
    { 0x00,  16, 133, { 54, 56, -1, -1, -1, -1 }},  /* 55 */
    { 0x37,  11, 122, { 55, 57, -1, -1, -1, -1 }},  /* 56 */
    { 0x03,  10, 113, { 56, 58, -1, -1, -1, -1 }},  /* 57 */
    { 0x02,  11, 102, { 57, 59, -1, -1, -1, -1 }},  /* 58 */
    { 0x37,  12,  92, { 58, 60, -1, -1, -1, -1 }},  /* 59 */
    { 0x05,  16,  82, { 59, 66, -1, -1, -1, -1 }},  /* 60 */
    { 0x01,  88,  97, {  0, 62, -1, -1, -1, -1 }},  /* 61 */
    { 0x03,  76,  91, { 61, 63, -1, -1, -1, -1 }},  /* 62 */
    { 0x00,  63,  86, { 62, 64, -1, -1, -1, -1 }},  /* 63 */
    { 0x02,  52,  79, { 63, 65, -1, -1, -1, -1 }},  /* 64 */
    { 0x05,  40,  74, { 64, 66, -1, -1, -1, -1 }},  /* 65 */
    { 0x14,  25,  67, { 65, 67, 60, -1, -1, -1 }},  /* 66 */
    { 0x05,  35,  53, { 66, 68, -1, -1, -1, -1 }},  /* 67 */
    { 0x37,  44,  45, { 67, 69, -1, -1, -1, -1 }},  /* 68 */
    { 0x00,  55,  39, { 68, 70, -1, -1, -1, -1 }},  /* 69 */
    { 0x01,  67,  33, { 69, 71, -1, -1, -1, -1 }},  /* 70 */
    { 0x37,  79,  29, { 70, 72, -1, -1, -1, -1 }},  /* 71 */
    { 0x02,  92,  26, { 71,  6, -1, -1, -1, -1 }}   /* 72 */
};

static void Question(void);
static Word TrouveTheme(void);
static void Explore(Byte, Byte, Word);


void ClickBureau(void)
{
    Word i;
    Rect r;

    if (pJeu.etape == CHOISIR_CASE)
        for (i = 0; i < pJeu.ndest; i++)
            if (abs(cases[pJeu.dest[i]].x - tache.where.h) < DMAX
                && abs(cases[pJeu.dest[i]].y - tache.where.v) < DMAX) {
                pJeu.joueur[pJeu.tour].position = pJeu.dest[i];
                pJeu.etape = LANCER_DE;
                RedessineFond();
                if (cases[pJeu.dest[i]].type >> 4 != 3)
                    Question();
                pJeu.de = 6;
                SetPort(joueursDialog);
                SetRect(&r, 25, 145, 70, 181);
                InvalRect(&r);
            }
}

static void Question(void)
{
    Word pos = pJeu.joueur[pJeu.tour].position, theme;

    if (DoQuestRep((theme = TrouveTheme()) + 1)) {
        pJeu.joueur[pJeu.tour].total[theme].succes++;
        switch (cases[pos].type >> 4) {
            case 1 :                        /* camembert */
                {
                    Rect r;

                    if (!(pJeu.joueur[pJeu.tour].camemberts & 1 << theme)) {
                        pJeu.joueur[pJeu.tour].camemberts |= 1 << theme;
                        SetPort(joueursDialog);
                        GetPortRect(&r);
                        InvalRect(&r);
                        if (sonActif)
                             JoueSon(&clap, 100);
                        if (pJeu.joueur[pJeu.tour].camemberts == 0x003f)
                             Dialogue6Camemberts();
                    }
                }
                break;
            case 2 :                        /* case centrale */
                if (pJeu.joueur[pJeu.tour].camemberts == 0x003f) {
                /*  FinPartie();  */
                    if (!pJeu.gagnee)
                        DialogueBravo();
                    pJeu.gagnee = TRUE;
                    SetDItemValue(pJeu.joueur[pJeu.tour].enJeu = FALSE,
                                                    joueursDialog, pJeu.tour+1);
                    JoueurSuivant();
                }
        }
    } else {
        if ((cases[pos].type >> 4)==1 && sonActif)
                JoueSon(&decu, 215);
        JoueurSuivant();
      }
}

static Word TrouveTheme(void)
{
    Word pos = pJeu.joueur[pJeu.tour].position, theme;

    if (cases[pos].type >> 4 == 2)  /* case centrale */
        if (pJeu.joueur[pJeu.tour].camemberts == 0x003f && !pJeu.gagnee) {
            /* question finale */
            Word minimum = 0xffff, i, d;

            d = rand() % 6;
            for (i = 0; i < 6; i++) {
                if (pJeu.joueur[pJeu.tour].total[d % 6].essai < minimum) {
                    theme = d % 6;
                    minimum = pJeu.joueur[pJeu.tour].total[d % 6].essai;
                }
                d++;
            }
            DialogueFinal();
        } else
            theme = ChoixTheme();
    else
        theme = cases[pos].type & 0x0f;
    pJeu.joueur[pJeu.tour].total[theme].essai++;
    return theme;
}

void Chemins(Byte pos, Word dep)
{
    pJeu.ndest = 0;
    Explore(pos, pos, dep);
}

static void Explore(Byte pos, Byte org, Word dep)
{
    Word i;
    Byte c;

    if (dep)
        for (i = 0; i < 6;) {
            c = cases[pos].adj[i++];
            if (c != 0xff && c != org)
                Explore(c, pos, dep-1);
        }
    else
        pJeu.dest[pJeu.ndest++] = pos;
}

void RedessineFond(void)
{
    Rect r;

    SetRect(&r, 0, 0, 221, 200);
    RefreshDesktop(&r);
}

pascal void DessineFond(void)
{
    static QDIconRecordPtr face[6] = {
        &homme1Icon,
        &femme1Icon,
        &homme2Icon,
        &femme2Icon,
        &homme3Icon,
        &femme3Icon
    };
    static QDIconRecordPtr pion[8] = {
        &pion1Icon,
        &pion2Icon,
        &pion3Icon,
        &pion4Icon,
        &pion5Icon,
        &pion6Icon,
        &pionCentreIcon,
        &pionDeIcon
    };
    Word i;
    Rect r;
    LocInfo source;

    /* Preserve data bank (see note in Toolbox Reference vol.2 p.25-43) */
    asm {
        phb
        phk
        plb
    }
    HLock((Handle)imageHdl);
    source.portSCB = 0;
    source.ptrToPixImage = (Pointer)*imageHdl;
    source.width = 160;
    SetRect(&source.boundsRect, 0, 0, 320, 200);
    SetRect(&r, 0, 0, 320, 187);
    PPToPort(&source, &r, 0, 13, modeCopy);
    HUnlock((Handle)imageHdl);
    DrawIcon((Pointer)face[pJeu.tour], 0,
        cases[pJeu.joueur[pJeu.tour].position].x - face[pJeu.tour]->iconWidth / 2,
        cases[pJeu.joueur[pJeu.tour].position].y - face[pJeu.tour]->iconHeight / 2);
    if (pJeu.etape == CHOISIR_CASE)
        for (i = 0; i < pJeu.ndest; i++)
            DrawIcon((Pointer)pion[cases[pJeu.dest[i]].type & 0x0f], 0,
                            cases[pJeu.dest[i]].x-10, cases[pJeu.dest[i]].y-8);
    // Restore data bank
    asm {
        plb
    }
}
