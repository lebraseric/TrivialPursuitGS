/*
 *  main.1 -- Module principal de Trivial Pursuit
 */


/*  Headers application  */

#include "trivial.h"
#include "initdesk.h"
#include "typedefs.h"
#include "desk.h"
#include "sounds.h"
#include "os.h"
#include "dialogs.h"

/*  Headers C standards  */

#include <string.h>
#include <stdlib.h>


/*  Headers Toolbox  */

#include <types.h>
#include <memory.h>
#include <misctool.h>
#include <quickdraw.h>
#include <font.h>
#include <event.h>
#include <window.h>
#include <menu.h>
#include <dialog.h>
#include <desk.h>
#include <gsos.h>
// #include <mjuke.h>


char titreActif[]="\pSon actif";
char titreInactif[]="\pSon inactif";
Boolean sonActif= TRUE;
WmTaskRec tache;
tInfoRec InfoRec;
Jeu jeu;
PJeu pJeu = { 3, 6, 0, 0 };
Ecran **imageHdl;


static char *Menus[] = {
">> Questions \\N5\r\
--Facile \\N270*11\r\
--Moyen \\N271*22\r\
--Expert \\C\22N272*33\r\
--Incollable \\VN277*44\r\
--Edition... \\N273\r\
--Informations \\N276*Ii\r",
">> Jeu \\N4\r\
--Noms... \\N274\r\
--Scores \\VN275*==\r\
--Son actif \\N278\r",
">> Edition \\DN3\r\
--Annuler \\VN250*Zz\r\
--Couper \\N251*Xx\r\
--Copier \\N252*Cc\r\
--Coller \\VN253*Vv\r\
--Effacer \\N254\r",
">> Fichier \\N2\r\
--Nouveau jeu... \\*NnN258\r\
--Ouvrir... \\VN259*Oo\r\
--Sauver \\N260*Ss\r\
--Sauver sous... \\VN261\r\
--Quitter \\N257*Qq\r",
">>@\\N1X\r\
--A propos de... \\N256\r\
--Instructions... \\VN262*??\r"
};


static Boolean nonFini = TRUE;
static Pointer border = (Pointer)0x00e1c034;
static int old_border;


static void ByeBye(void);
static void InitFont(void);
static void ActivationFen(void);
static void ExecMenu(int art);
static void DoQuitter(void);
static void DoNouveau(void);
static void DoSetDiff(int article);
static void PlaceMenus(int NbreMenus);
static void DecompacteFond(void);
static void ColorCycle(void);

main()
{
    int code, rc, Init_Defaut, Init_Get;

    old_border = *border;
    *border = 0x04;
    atexit(ByeBye);
    if (rc = InitTools(mode320))
        exit(rc);
    srand((word)TickCount());   /* init du generateur pseudo-aleatoire */
    PlaceMenus(5);
    DecompacteFond();
    SetColorTable(0, (**imageHdl).tableCoul[0]);
    Desktop(SetDeskPat, (Long)DessineFond);
    // MJMount("\p9/ABOUT.MJK", 1);
    InitFont();
    Init_Defaut = initBase("9:bases:genus", &InfoRec);
    if (Init_Defaut)
        Init_Get = OuBase();
    if ((!Init_Defaut) || (!Init_Get)) {
        InitJoueursDialog();
        jeu.action = FALSE;
        DoNouveau();
        ColorCycle();
        InitCursor();
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
                    ExecMenu((int)tache.wmTaskData);
                    break;
                case keyDownEvt :
                    if ((tache.message & 0xff) == '\r') {
                        LanceDe();
                        jeu.action = TRUE;
                    }
                    break;
            }
            if (IsDialogEvent(&tache))
                DoModelessEvent();
        } while (nonFini);
    }
}

void ByeBye(void)
{
    fermeBase();
    *border = old_border;
}

void InitFont(void)
{
    InstallFont((FontID){ geneva, 0, 10 }, 0);
    InstallFont((FontID){ geneva, 0, 12 }, 0);
    InstallFont((FontID){ venice, 0, 14 }, 0);
    InstallFont((FontID){ times, 0, 9 }, 0);
    InstallFont((FontID){ shaston, 0, 8 }, 0);
}

void ActivationFen(void)
{
    if (tache.modifiers & activeFlag)
        SetMenuFlag(disableMenu, 3);
    else
        SetMenuFlag(enableMenu, 3);
    DrawMenuBar();
}

void ExecMenu(int art)
{
    switch (art) {
        case 256 : DoAbout(); break;
        case 257 : DoQuitter(); break;
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

void DoQuitter(void)
{
    if (jeu.action)
        Fermer();
    if (!jeu.action)
        nonFini = FALSE;
}

void DoNouveau(void)
{
    Word i;

    if (jeu.action)
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

void DoSetDiff(int article)
{
    switch (article) {
        case 270 : pJeu.diff = 1; break;
        case 271 : pJeu.diff = 2; break;
        case 272 : pJeu.diff = 3; break;
        case 277 : pJeu.diff = 4;
    }
    Coche();
}

void PlaceMenus(int NbreMenus)
{
    int i;
    for (i = 0; i < NbreMenus; i++)
        InsertMenu(NewMenu(Menus[i]), 0);
    FixAppleMenu(1);
    FixMenuBar();
    DrawMenuBar();
}

void DecompacteFond(void)
{
    Word size = 0x8000;
    Ecran *AdrImage;

    imageHdl = (Ecran **)NewHandle(0x8000L, MyID, 0x0010, 0L);
    if (_toolErr)
        SysErr();
    HLock((Handle)imageHdl);
    AdrImage = *imageHdl;
    UnPackBytes(fondEcran.data, fondEcran.length, (Handle)&AdrImage, &size);
    HUnlock((Handle)imageHdl);
}

void ColorCycle(void)
{
    Word i, j, a;
    LongWord dt;

    for (i = 0; i < 12; i++) {
        a = (**imageHdl).tableCoul[0][1];
        for (j = 1; j <= 6; j++)
            (**imageHdl).tableCoul[0][j] =
                                        (**imageHdl).tableCoul[0][j+1];
        (**imageHdl).tableCoul[0][6] = a;
        SetColorTable(0, (**imageHdl).tableCoul[0]);
        dt = TickCount() + 6L;
        while (TickCount() < dt);  /* delai 1/10 s */
    }
}
