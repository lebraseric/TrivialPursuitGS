/*
 *  desk.1 -- Mise a jour fenetre joueurs
 */


/*  Header application  */

#include "trivial.h"


/*  Headers Toolbox  */

#include <types.h>
#include <quickdraw.h>
#include <event.h>
#include <control.h>
#include <window.h>
#include <dialog.h>
#include <font.h>
#include <qdaux.h>
#include <gsos.h>


/*  Donnees de main.1  */

extern WmTaskRec tache;
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


/*  Donnees de ticons.1  */

extern QDIconRecord partIcon;
extern QDIconRecord Un, Deux, Trois, Quatre, Cinq, Six, Cliquer;


/*  Donnees de sons.1  */

extern DataBlock dice;


/*  Variables statiques  */

GrafPortPtr joueursDialog;
Rect deRect = { 153, 33, 176, 63 };


void InitJoueursDialog()
{
    static Rect jFenBoundsRect = { 18, 223, 199, 319 };
    static WindColor jFenColors = { 0x0050, 0, 0, 0 };
    static Rect partsRect = { 16, 20, 149, 95 };
    static BoxColors jCaseColors = { 0, 0xed, 0xe8, 0xed };
    int i, j;
    Rect r;

    SetPort(joueursDialog = NewModelessDialog(&jFenBoundsRect,
                                              NULL,
                                              0xffffffff,
                                              fQContent + fCtlTie,
                                              0L,
                                              NULL));
    SetFrameColor(&jFenColors, joueursDialog);
    SetSolidBackPat(14);
    InstallFont(10*256+0, geneva, 0);
    NewDItem(joueursDialog, DE, &deRect, userItem, DessineDe, 0, 0, NULL);
    NewDItem(joueursDialog, PARTS, &partsRect, userItem, DessineParts, 0, 0, NULL);
    for (i = 5; i >= 0; i--) {
        SetRect(&r, 2, 2+i*25, 0, 0);
        NewDItem(joueursDialog, i+1, &r, checkItem,
                                    pJeu.joueur[i].nom, FALSE, 0, &jCaseColors);
    }
    SetContentDraw(ContenuJFen, joueursDialog);
    ShowWindow(joueursDialog);
}

void DoModelessEvent()
{
    GrafPortPtr result;
    Word itemHit;

    if (DialogSelect(&tache, &result, &itemHit))
        switch (itemHit) {
            case 1 :
            case 2 :
            case 3 :
            case 4 :
            case 5 :
            case 6 :
                SetDItemValue(pJeu.joueur[itemHit-1].enJeu =
                            !GetDItemValue(result, itemHit), result, itemHit);
                if (!pJeu.joueur[itemHit-1].enJeu && itemHit == pJeu.tour+1)
                    JoueurSuivant();
                break;
            case 11 :
                LanceDe();
                jeu.action = TRUE;
        }
}

void LanceDe()
{
    Word tours, oldDe = 0xffff;
    LongWord dt;

    if (pJeu.etape == LANCER_DE) {
        dt = 0;
        for (tours = 3 + rand() % 6; tours > 0; tours--) {
            while (TickCount() < dt);  /* delai */
            do
                pJeu.de = rand() % 6;
            while (pJeu.de == oldDe);
            DessineDe(joueursDialog, DE);
            JoueSon(&dice, 200, 1);
            dt = TickCount() + (long)(4 + rand() % 20);
            oldDe = pJeu.de;
        }
        Chemins(pJeu.joueur[pJeu.tour].position, pJeu.de+1);
        pJeu.etape = CHOISIR_CASE;
        RedessineFond();
    }
}

void RedessineJFen()
{
    Rect r;
    Word i;

    SetPort(joueursDialog);
    GetPortRect(&r);
    EraseRect(&r);
    InvalRect(&r);
    for (i = 0; i < 6; i++) {
        if (pJeu.joueur[i].nom[0])
            ShowControl(GetControlDItem(joueursDialog, i+1));
        else
            HideControl(GetControlDItem(joueursDialog, i+1));
        SetDItemValue(pJeu.joueur[i].enJeu, joueursDialog, i+1);
    }
}

void ContenuJFen()
{
    Rect r;
    Word i, n;

    DrawControls(joueursDialog);
    for (i = 0, n = 0; i < 6; i++)
        if (pJeu.joueur[i].nom[0])
            n++;
    SetRect(&r, 17, 2, 94, 16);
    for (i = 0; i < 6; i++) {
        SetSolidPenPat(i == pJeu.tour && n > 1 ? 8 : 14);
        FrameRect(&r);
        OffsetRect(&r, 0, 25);
    }
}

pascal void DessineParts(dialog, item)
GrafPortPtr dialog;
Word item;
{
    int i, j;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6;)
            if (pJeu.joueur[i].camemberts & 1 << j++)
                DrawIcon(&partIcon, 0xe000 + j*256, j*10 + 10, i*25 + 17);
}

pascal void DessineDe(dialog, item)
GrafPortPtr dialog;
Word item;
{
    static QDIconRecordPtr Des[7] = {&Un, &Deux, &Trois, &Quatre, &Cinq, &Six, &Cliquer};
    Rect r;
    Point oldPenSize;

    DrawIcon(Des[pJeu.de], 0, deRect.h1, deRect.v1);
    GetPenSize(&oldPenSize);
    SetPenSize(3, 3);
    SetSolidPenPat(pJeu.de == 6 ? 2 : 14);
    SetRect(&r, 29, 149, 67, 180);
    FrameRRect(&r, 16, 16);
    SetPenSize(oldPenSize);
}

void JoueurSuivant()
{
    Word i;
    Rect r;

    for (i = 0; i < 6; i++) {
        pJeu.tour++;
        if (pJeu.tour > 5)
            pJeu.tour = 0;
        if (pJeu.joueur[pJeu.tour].nom[0] && pJeu.joueur[pJeu.tour].enJeu)
            break;
    }
    pJeu.etape = LANCER_DE;
    pJeu.de = 6;
    SetPort(joueursDialog);
    GetPortRect(&r);
    InvalRect(&r);
    RedessineFond();
}
