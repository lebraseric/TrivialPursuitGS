/*
 *  game.c -- Game routines
 */

#include "trivial.h"
#include "typedefs.h"
#include "game.h"
#include "initdesk.h"
#include "dialogs.h"
#include "jfen.h"
#include "desk.h"

#include <types.h>
#include <string.h>

PJeu pJeu = { 3, 6, 0, 0 };

void DoNouveau(void)
{
    Word i;

    if (jeu.action) // Game already running
        Fermer();
    if (!jeu.action) {
        for (i = 0; i < 6; i++) {
            *pJeu.joueur[i].nom
                = pJeu.joueur[i].camemberts
                = pJeu.joueur[i].position
                = pJeu.joueur[i].enJeu
                = 0;
            memset(pJeu.joueur[i].total, 0, sizeof(pJeu.joueur[i].total));
        }
        strcpy(pJeu.joueur[0].nom, "\pJoueur 1");
        pJeu.joueur[0].enJeu = TRUE;
        jeu.nouveau = TRUE;
        pJeu.gagnee = FALSE;
        pJeu.tour = 0;
        pJeu.etape = LANCER_DE;
        RedessineJFen();
        RedessineFond();
    }
}

