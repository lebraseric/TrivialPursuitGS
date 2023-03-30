/*
 *  main.1 -- Module principal de Trivial Pursuit
 */

/*  Headers application  */

#include "trivial.h"
#include "typedefs.h"
#include "initdesk.h"
#include "desk.h"
#include "sounds.h"
#include "os.h"
#include "dialogs.h"

/*  Headers C standards  */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*  Headers Toolbox  */

#include <types.h>
#include <event.h>
#include <window.h>
#include <menu.h>
#include <dialog.h>

char titreActif[]="\pSon actif";
char titreInactif[]="\pSon inactif";
Boolean sonActif= TRUE;
WmTaskRec tache;

static void event_loop(void);
static void ActivationFen(void);
static void ExecMenu(const int art, int *quit);
static void DoSetDiff(int article);

int main()
{
    int result;
    const Pointer border = (Pointer)0x00e1c034;
    uint8_t old_border;

    old_border = *border;
    *border = 0x04; // Dark green border
    if ((result = init_startup()) > 0)
        return result;
    event_loop();
    fermeBase();
    sp_done();
    *border = old_border;
}

static void event_loop(void)
{
    int code;
    int quit;

    tache.wmTaskMask = 0x1FFF;
    FlushEvents(everyEvent & ~mDownMask & ~mUpMask, 0);
    do {
        if (!(code = TaskMaster(everyEvent, &tache)))
            continue;
        switch (code) {
            case activateEvt :
                ActivationFen();
                break;
            case wInDesk :
                ClickBureau();
                break;
            case wInMenuBar :
                ExecMenu((int)tache.wmTaskData, &quit);
                if (quit)
                    return;
                break;
            case keyDownEvt :
                if ((tache.message & 0xff) == '\r') {
                    LanceDe();
                }
                break;
        }
        if (IsDialogEvent(&tache))
            DoModelessEvent();
    } while (true);
}

static void ActivationFen(void)
{
    if (tache.modifiers & activeFlag)
        SetMenuFlag(disableMenu, 3);
    else
        SetMenuFlag(enableMenu, 3);
    DrawMenuBar();
}

static void ExecMenu(const int art, int *quit)
{
    switch (art) {
        case 256 : DoAbout(); break;
        case 257 :
            if (jeu.action)
                Fermer();
            *quit = !jeu.action;
            break;
        case 258 : DoNouveau(); break;
        case 259 : DoOuvrir(); break;
        case 260 : DoSauver(); break;
        case 261 : DoSauverSous(); break;
        case 262 : DoInstr(); break;
        case 270 :
        case 271 :
        case 272 :
        case 277 : DoSetDiff(art); break;
        case 273 : DoEdition(); break;
        case 274 : DoNoms(); break;
        case 275 : DoScores(); break;
        case 276 : DoInfos(); break;
        case 278 : DoActiveSon(); break;
    }
    HiliteMenu(FALSE, (int)(tache.wmTaskData >> 16));
}

static void DoSetDiff(int article)
{
    switch (article) {
        case 270 : pJeu.diff = 1; break;
        case 271 : pJeu.diff = 2; break;
        case 272 : pJeu.diff = 3; break;
        case 277 : pJeu.diff = 4;
    }
    check_difficulty_menu();
}
