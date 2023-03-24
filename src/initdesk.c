/* ************************************************************************* */
/*                                                                           */
/* INITDESK.1 - Debut et fin standard d'une application desktop              */
/*                                                                           */
/* Version 0.2    13/11/89                                                   */
/*                                                                           */
/* ************************************************************************* */


#include "initdesk.h"

#include <stdlib.h>
#include <LOCATOR.H>
#include <MEMORY.H>
#include <INTMATH.H>
#include <SOUND.H>
// #include <MJUKE.H>


Word MyID;
// Word MJStart=0;

static Ref ToolRecRef;
static Pointer Video = (Pointer)0xc029;
static char NullStr[] = "\p";

static void CloseTools(void);
static void ErrorCheck(int);


/* ************************************************************************* */
/*                                                                           */
/* InitTools - Demarre les outils necessaires a l'application                */
/*                                                                           */
/* ************************************************************************* */

Word InitTools(Word mode)
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
   int ResultCode;
   char *z;
   char **pgzero;

   toolTable.InitMode = mode;
   TLStartUp();
   ErrorCheck(1);
   if (TLVersion() >= 0x0300) {
       MyID = MMStartUp();
       ErrorCheck(2);
    //    LoadOneTool(69, 101);
    //    if (!_toolErr) {
    //           MJStart=1;
    //           MJStartUp(MyID, 0);
    //    }
    //    else {
    //           pgzero = NewHandle(256L,MyID,0xC00C,0L);
    //           z = *pgzero;
    //           SoundStartUp((int) z);
    //           TLTextMountVolume(NullStr, "\pTool 069 introuvable", "\pContinuer", "\p");
    //    }
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
