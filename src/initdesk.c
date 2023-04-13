/* ************************************************************************* */
/*                                                                           */
/* INITDESK.1 - Debut et fin standard d'une application desktop              */
/*                                                                           */
/* Version 0.2    13/11/89                                                   */
/*                                                                           */
/* ************************************************************************* */


#include "initdesk.h"
#include "trivial.h"
#include "typedefs.h"
#include "sp.h"
#include "desk.h"
#include "os.h"
#include "dialogs.h"
#include "jfen.h"
#include "game.h"

#include <event.h>
#include <quickdraw.h>
#include <menu.h>
#include <desk.h>
#include <misctool.h>
#include <window.h>
#include <font.h>
#include <LOCATOR.H>
#include <memory.h>
#include <INTMATH.H>
#include <SOUND.H>
// #include <MJUKE.H>
#include <stdlib.h>
#include <string.h>


Word MyID;
Ecran **imageHdl;
tInfoRec InfoRec;
Jeu jeu;
// Word MJStart=0;

static Ref ToolRecRef;
static Pointer Video = (Pointer)0xc029;
static char NullStr[] = "\p";

static Word InitTools(Word mode);
static void ErrorCheck(int);
static void CloseTools(void);
static void init_menus(void);
static void unpack_background(void);
static void init_fonts(void);
static void cycle_colors(void);

int init_startup(void)
{
    int result;

    if ((result = InitTools(mode320)) > 0)
        return result;
    sp_init();
    srand((word)TickCount());   // Set random generator seed
    init_menus();
    unpack_background();
    SetColorTable(0, (**imageHdl).tableCoul[0]);
    Desktop(SetDeskPat, (Long)DessineFond);
    init_fonts();
    memset(&InfoRec, 0, sizeof(InfoRec));
    result = initBase("9:bases:genus", &InfoRec);
    if (result > 0)
        result = OuBase();
    InitJoueursDialog();
    jeu.action = FALSE;
    DoNouveau();
    cycle_colors();
    InitCursor();
    return 0;
}

/* ************************************************************************* */
/*                                                                           */
/* InitTools - Demarre les outils necessaires a l'application                */
/*                                                                           */
/* ************************************************************************* */

static Word InitTools(Word mode)
{
   static struct {
                   Word Flag;
                   Word InitMode;
                   Word ResFileID;
                   long dPageHandle;
                   Word Length;
                   Word Table [19*2];
                 }
   toolTable =
                 {
                   0,
                   0x4000,         /* Mode d'initialisation de QuickDraw */
                   0,
                   0,
                   16,             /* Length of this list - number of items */
                    3,0x0101,      /* Misc Tools      */
                    4,0x0101,      /* QuickDraw II    */
                    5,0x0100,      /* Desk Manager    */
                    6,0x0100,      /* Event Manager   */
                    7,0x0100,      /* Scheduler       */
                   11,0x0100,      /* Integer Math    */
                   14,0x0103,      /* Window Manager  */
                   15,0x0103,      /* Menu Manager    */
                   16,0x0103,      /* Control Manager */
                   18,0x0100,      /* QuickDraw Aux   */
                   20,0x0100,      /* Line Edit       */
                   21,0x0100,      /* Dialog Manager  */
                   22,0x0102,      /* Scrap Manager   */
                   23,0x0100,      /* Standard File   */
                   27,0x0100,      /* Font Manager    */
                   28,0x0100,      /* List Manager    */
                  };            /* Tool Locator data structure */
   char *z;
   char **pgzero;

   toolTable.InitMode = mode;
   TLStartUp();
   ErrorCheck(1);
   if (TLVersion() >= 0x0300) {
       MyID = MMStartUp();
       ErrorCheck(2);
       pgzero = NewHandle(256L,MyID,0xC00C,0L);
       z = *pgzero;
       SoundStartUp((int) z);
       ToolRecRef = StartUpTools(MyID, 0, (Ref)&toolTable);
       ErrorCheck(3);
       atexit(CloseTools);
       return 0;
   }
   else {
       TLShutDown();
       *Video = 0x41;                          /* Restaure le mode texte */
       TLTextMountVolume(
           "\pCe programme n{cessite le",
           "\psyst}me 5.0 ou mieux.",
           "\pQuitter",
           NullStr);
       return 4;
   }
}

void check_difficulty_menu(void)
{
    static int check = 272;
    int a;

    switch (pJeu.diff) {
        case 1 : a = 270; break;
        case 2 : a = 271; break;
        case 3 : a = 272; break;
        case 4 : a = 277; break;
    }
    CheckMItem(false, check);
    CheckMItem(true, check = a);
}

/* ************************************************************************* */
/*                                                                           */
/* ErrorCheck - Affiche un dialogue et quitte l'application en cas d'erreur  */
/*                                                                           */
/* ************************************************************************* */

static void ErrorCheck(int where)
{
   static char Texte[] = "\pErreur fatale $0000 en 0000";
   if (_toolErr) {
       Int2Hex(_toolErr, Texte + 16, 4);
       Int2Hex(where, Texte + 24, 4);
       *Video = 0x41;                          /* Restaure le mode texte */
       TLTextMountVolume(NullStr, Texte, "\pQuitter", "\p");
       switch (where) {
           case 2 : MMShutDown(MyID);
           case 1 : TLShutDown(); break;
           default : CloseTools();
       }
       exit(where);
   }
}

/* ************************************************************************* */
/*                                                                           */
/* CloseTools - Ferme les outils ouverts par InitTools().                    */
/*                                                                           */
/* ************************************************************************* */

static void CloseTools(void)
{
   ShutDownTools(0, ToolRecRef);

//    if (MJStart)
//         MJShutDown();
//    else
        SoundShutDown();

   MMShutDown(MyID);
   TLShutDown();
}

static void init_menus(void)
{
    const char *menus[] = {
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

    int i;
    for (i = 0; i < sizeof(menus)/sizeof(menus[0]); i++)
        InsertMenu(NewMenu(menus[i]), 0);
    FixAppleMenu(1);
    FixMenuBar();
    DrawMenuBar();
}

static void unpack_background(void)
{
    Word size = 0x8000;
    Ecran *AdrImage;

    imageHdl = (Ecran **)NewHandle(size, MyID, 0x0010, 0L);
    if (_toolErr)
        SysErr();
    HLock((Handle)imageHdl);
    AdrImage = *imageHdl;   // This pointer will be modified by UnPackBytes
    UnPackBytes(fondEcran.data, fondEcran.length, (Handle)&AdrImage, &size);
    HUnlock((Handle)imageHdl);
}

static void init_fonts(void)
{
    InstallFont((FontID){ geneva, 0, 10 }, 0);
    InstallFont((FontID){ geneva, 0, 12 }, 0);
    InstallFont((FontID){ venice, 0, 14 }, 0);
    InstallFont((FontID){ times, 0, 9 }, 0);
    InstallFont((FontID){ shaston, 0, 8 }, 0);
}

static void cycle_colors(void)
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

/* ************************************************************************* */
/*                                                                           */
/* SysErr - Traitement erreur systeme                                        */
/*                                                                           */
/* ************************************************************************* */

void SysErr(void)
{
    static char texte[] = "\pErreur systeme $0000";
    if (_toolErr) {
        Int2Hex(_toolErr, texte + 17, 4);
        *Video = 0x41;   /* passage en mode texte */
        switch (TLTextMountVolume(NullStr, texte, "\pQuitter", "\pContinuer")) {
            case 1 : exit(-1);
                     break;
            case 2 : *Video = 0xa1; /* Restaure le mode SHR */
                     break;
        }
    }
}
