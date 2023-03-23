/*
 *  dialogs.2 -- Gestion des dialogues de Trivial Pursuit (Version Speech)
 */


/*  Header application  */

#include "trivial.h"

/* Headers SpeechToolKit */

#include <spchparser.h>
#include <spchmale.h>
#include <spchfemale.h>

/*  Headers C standards  */

#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>


/*  Headers Toolbox  */

#include <types.h>
#include <quickdraw.h>
#include <window.h>
#include <dialog.h>
#include <stdfile.h>
#include <font.h>
#include <qdaux.h>
#include <gsos.h>
#include <mjuke.h>


/*  Donnees de main.1  */

extern Boolean sonActif;

extern tInfoRec InfoRec;
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


/*  Donnees de jfen.1  */

extern GrafPortPtr joueursDialog;


/*  Donnees de dlgdata.1  */

extern DialogTemplate infosTemp;
extern DialogTemplate themeTemp;
extern StringPtr themeBut1,themeBut2,themeBut3,themeBut4,themeBut5,themeBut6;


/*  Donnees de ticons.1  */

extern QDIconRecord camembertIcon;

/* Donnees de sons.1 */
extern DataBlock yeah;

int OuBase()
{
    SFReplyRec      Base;
    /* char            CheminBase[129]; */
    int             filterBase();

    SFGetFile(30,45,"\pO\235 se trouve la base de questions ?",filterBase, NIL,&Base);
    if (Base.good) {
        p2cstr(Base.fullPathname);
        initBase(Base.fullPathname, &InfoRec);
        return 0;
    } else
        return 1;
}

pascal int filterBase(Dir)
long  Dir;
{
        if ((*(word*)(Dir+0x10)==0xF5) && (*(word*)(Dir+0x1f)==0x6367))
                return (displaySelect);
        else
                return (noDisplay);
}

void DoOuvrir()
{
    SFReplyRec choix;
    int filterOuvrir();

    if (jeu.action)
        Fermer();
    if (!jeu.action) {
         SFGetFile(30, 45, "\pOuvrir le jeu :", filterOuvrir, NIL, &choix);
         if (choix.good) {
             p2cstr(choix.filename);
             p2cstr(choix.fullPathname);
             strcpy(jeu.nom, choix.filename);
             strcpy(jeu.chemin.text, choix.fullPathname);
             jeu.chemin.length = strlen(jeu.chemin.text);
             Ouvrir();
             jeu.nouveau = FALSE;
             jeu.action = FALSE;
             Coche();
             RedessineJFen();
             RedessineFond();
         }
    }
}

pascal int filterOuvrir(Dir)
long  Dir;
{
        if ((*(word*)(Dir+0x10)==0xF6) && (*(word*)(Dir+0x1f)==0x6367))
                return (displaySelect);
        else
                return (noDisplay);
}

void DoSauver()
{
    if (jeu.nouveau)
        DoSauverSous();
    else
        Enregistrer();
}

void DoSauverSous()
{
    SFReplyRec choix;

    SFPutFile(25, 45, "\pSauver le jeu sous :", "\pSans.titre", 15, &choix);
    if (choix.good) {
        p2cstr(choix.filename);
        p2cstr(choix.fullPathname);
        strcpy(jeu.nom, choix.filename);
        strcpy(jeu.chemin.text, choix.fullPathname);
        jeu.chemin.length = strlen(jeu.chemin.text);
        Enregistrer();
        jeu.nouveau = FALSE;
    }
}

void DoInstr()
{
   GrafPortPtr  CurPort,dlgInstr;
   word         itemHit;
   int          page=0;
   void         InstrPage();

   static ItemTemplate annuler = {
        2,
        {158,117,172,202},
        buttonItem,
        "\pFin",
        0,
        3,
        NIL
   };
   static ItemTemplate suivant = {
        3,
        {158,213,172,292},
        buttonItem,
        "\pSuivant",
        0,
        3,
        NIL
   };
   static ItemTemplate prec = {
        4,
        {158,4,172,105},
        buttonItem,
        "\pPr\216c\216dent",
        0,
        3,
        NIL
   };

   static DialogTemplate tmpInstr = {
         { 18,8,195,312 },
         FALSE,
         0L,{
             &prec,
             &suivant,
             &annuler,
             0L
         }
   };

   CurPort = GetPort();
   SetPort(dlgInstr = GetNewModalDialog(&tmpInstr));

   while (itemHit!=2) {

    if (page>0) ShowDItem(dlgInstr,4);
        else HideDItem(dlgInstr,4);
    if (page==8) HideDItem(dlgInstr,3);
    else ShowDItem(dlgInstr,3);

    SetSolidBackPat(7);
    ShowWindow(dlgInstr);

        InstrPage(page);

        InstallFont(8*256+0, shaston, 0);
        itemHit = ModalDialog(NIL);
        if (itemHit==3) page++;
        if (itemHit==4) page--;

   }
   SetPort(CurPort);
   CloseDialog(dlgInstr);
}

Word PageLen(pg)
char *pg;
{
        Word i;

        for (i = 0; *pg++ != 27; i++);
        return i;
}

void InstrPage(npage)
int npage;
{
        static char  numPageStr[2];
        Rect  r;

        static char *PageStr[]={
"\1S\5\0But du Jeu :\r\r\
\1S\0\0Le gagnant est celui qui arrive dans la case centrale apr\217s avoir \
r\216colt\216 les parts de camembert de chaque cat\216gorie et qui r\216pond \
alors correctement \210 la question pos\216e par l'ordinateur. Celui-ci \
choisira le th\217me en fonction des choix pr\216c\216dents du joueur : le \
th\217me le moins sollicit\216 !\r\
Pour r\216colter les parts de camembert, les joueurs doivent aller dans \
chacun des Quartiers G\216n\216raux des six cat\216gories et r\216pondre \
correctement aux questions pos\216es.\r\33",

"\1S\5\0D\216but de la partie : \r\r\
\1S\0\0Chaque joueur s'inscrit dans la liste des joueurs gr\211ce \210 \
l'option \1S\1\0Nom...\1S\0\0 du menu \1S\1\0Jeu\1S\0\0. \
Leurs noms apparaissent alors dans la fen\220tre \210 droite de l'\216cran, \
dans l'ordre \210 suivre pour le d\216roulement du jeu. \
Pour modifier le nom d'un joueur en cours de jeu, s\216lectionner \210 nouveau \
l'option \1S\1\0Nom...\1S\0\0 du menu \1S\1\0Jeu\1S\0\0. \
L'option \1S\1\0Nouveau Jeu...\1S\0\0 du menu \1S\1\0Fichier\1S\0\0 aura \
pour effet de remettre cette liste \210 z\216ro.\r\33",

"\rLe premier joueur clique sur le d\216, et en partant de la case \
centrale, avance son pion dans l'une des cases propos\216es par \
l'ordinateur.\r\
Quand un joueur arrive sur une case ou sur le QG d'une cat\216gorie, \
il se voit poser une question sous forme d'un dialogue. \
Le joueur peut \210 cet instant taper une r\216ponse et la valider, ou donner \
sa langue au chat et cliquer sur le bouton \42Je Ne Sais Pas\42.\33",

"Dans le cas d'une proposition du joueur, trois cas se pr\216sentent : \r \
- la r\216ponse est exacte, \r \
- la r\216ponse est \210 priori exacte, \r \
- la r\216ponse est \210 priori fausse \r\r \
Dans le dernier cas, le joueur peut proposer une r\216ponse \
jusqu'\210 ce que l'ordinateur analyse celle-ci comme exacte ou \
\210 priori exacte, ou bien appuyer sur le bouton \
\42Voir la R\216ponse\42.\33",

"Dans le cas d'une r\216ponse exacte ou si le joueur a cliqu\216 sur \
\42Voir la R\216ponse\42, la r\216ponse est donn\216e avec la \
possibilit\216 pour le joueur de confirmer si sa r\216ponse \
est bonne ou non. \
Il utilise pour cela les boutons \42J'ai Bon\42 ou \42J'ai Faux\42.\r\r \
Si la r\216ponse est correcte, le joueur rejoue en cliquant \210 \
nouveau sur le d\216. Sinon, c'est au tour du joueur suivant \
dans la liste de poursuivre le jeu.\r\33",

"\1S\5\0D\216roulement de la partie :\r\r\1S\0\0\
Quand un joueur r\216pond correctement \210 une question de Quartier G\216n\216ral \
de cat\216gorie, il voit appara\224tre sous son nom la part de camembert \
de la couleur correspondante. Si sa r\216ponse est inexacte, il doit quitter \
le QG au tour suivant.\r\
Un joueur qui atteint l'une des 12 cases marqu\216es d'un d\216 recommence \
en cliquant \210 nouveau sur le d\216.\r\33",

"Quand un joueur arrive dans la case centrale avant d'avoir r\216colt\216 \
toutes les parts de camembert de chaque cat\216gorie, cette case devient pour lui \
une case de libre choix, et un dialogue appara\224t o\235 le joueur choisit \
lui-m\220me le th\217me de la question.\r\
A chaque instant du jeu, il est possible de consulter un tableau des \
scores, qui donne par cat\216gorie le nombre de questions choisies \
par chacun des joueurs ainsi qu'un pourcentage global de r\216ussite ; \
c'est l'option \1S\1\0Scores...\1S\0\0 du menu \1S\1\0Jeu\1S\0\0.\33",

"Nous avons attribu\216 \210 chaque question un niveau de difficult\216 \
(tout \210 fait relatif !), qui permet gr\211ce aux diff\216rentes \
options du menu \1S\1\0Questions\1S\0\0 de rendre le jeu plus ou moins \
accessible et attractif \210 ceux que ce jeu rebute par sa difficult\216.\r\
\rUn joueur peut se retirer temporairement du jeu en cliquant sur la case \
figurant \210 c\231t\216 de son nom dans la fen\220tre de droite.\33",

"\1S\5\0Le gagnant :\r\r\1S\0\0\
Apr\217s avoir r\216pondu correctement \210 chaque question des six \
Quartiers G\216n\216raux, le joueur doit donc retourner \210 la case \
centrale pour essayer de r\216pondre \210 la question finale pos\216e par \
l'ordinateur. S'il conna\224t la r\216ponse \210 cette derni\217re question, \
il est d\216clar\216 vainqueur.\33"
};
        SetForeColor(13);
        SetBackColor(7);

        SetRect(&r,5,10,295,155);

        InstallFont(10*256+0, geneva, 0);
        sprintf(numPageStr, "%d", npage+1);

        LETextBox2(PageStr[npage], PageLen(PageStr[npage]), &r, 0);
        MoveTo(290,10);
        DrawCString(numPageStr);
}

void DoAbout()
{

    GrafPortPtr oldPort, dlgAbout, dlgInfo;
    int ItemHit;
    Rect rInfo;

    static char *InfoStr[]={
"\1J\1\0Nous remercions ici pour leur courage et pour leur \
aide inestimable : \
\r CLARIS, LAMBDA GS, JIHAILDE, PEPE GS \r\r \
Vous pouvez nous joindre sur \r 3614 code RTEL1, Bal Zubrowka \r \
pour toutes remarques ou suggestions, ainsi que pour nous signaler \
toute erreur de frappe ou d'orthographe \
dans les questions/r\216ponses ; il vous suffit de noter le num\216ro \
de la carte, le th\217me, la ou les fautes \216ventuelles, et de nous transmettre \
tout cela via le serveur. \r Nos amiti\216s \210 tous, et \r\r \
\1S\10\0Longue Vie au GS !!!\33"};

    static ItemTemplate bInfo = {
         2,
         { 110, 120, 125, 180 },
         buttonItem, "\pInfos...", 0, 0, 0L };

    static ItemTemplate bOk = {
         1,
         { 110,40,125,90 },
         buttonItem, "\pOk", 0, 2, 0L };

    static ItemTemplate bOkInfo = {
         1,
         { 155,120,170,170 },
         buttonItem, "\pOk", 0, 2, 0L };

    static DialogTemplate tmpAbout = {
         { 40, 48, 170, 272 },
         TRUE, 0L,
         { &bOk, &bInfo, 0L } };

    static DialogTemplate tmpInfo = {
         { 18, 8, 195, 312 },
         TRUE, 0L,
         { &bOkInfo, 0L } };

    MJPlay(1);

    SetRect(&rInfo,5, 5,295,160);

    oldPort = GetPort();
    SetPort(dlgAbout = GetNewModalDialog(&tmpAbout));

    SetForeColor(0);
    InstallFont(14*256, venice, 0);
    MoveTo(60, 25);
    DrawCString("Trivial Pursuit GS");
    InstallFont(8*256+0, shaston, 0);
    MoveTo(20, 75);
    DrawCString("par Zubrowka et le Traitre");
    MoveTo(4, 90);
    DrawCString("Version 1.01 - \42SpeechToolkit\42");
    DrawIcon(&camembertIcon, 0, 10, 10);
    ItemHit = ModalDialog(0L);
    CloseDialog(dlgAbout);

    if (ItemHit==2) {
        SetPort(dlgInfo = GetNewModalDialog(&tmpInfo));
        LETextBox2(*InfoStr, PageLen(*InfoStr), &rInfo, 0);
        ItemHit = ModalDialog(0L);
        CloseDialog(dlgInfo);
    }
    MJStop();
    SetPort(oldPort);
}

void DoNoms()
{
    static char nomStr[6][16];
    static ItemTemplate OK = {
        ok,
        {125, 136, 139, 221},
        buttonItem,
        "\pOK",
        0,
        0,
        NIL
    };
    static ItemTemplate annuler = {
        cancel,
        {125, 25, 139, 110},
        buttonItem,
        "\pAnnuler",
        0,
        0,
        NULL
    };
    static ItemTemplate nom1 = {
        51,
        {21, 25, 34, 172},
        editLine+itemDisable,
        nomStr[0],
        15,
        0,
        NULL
    };
    static ItemTemplate nom2 = {
        52,
        {37, 25, 50, 172},
        editLine+itemDisable,
        nomStr[1],
        15,
        0,
        NULL
    };
    static ItemTemplate nom3 = {
        53,
        {53, 25, 66, 172},
        editLine+itemDisable,
        nomStr[2],
        15,
        0,
        NULL
    };
    static ItemTemplate nom4 = {
        54,
        {69, 25, 82, 172},
        editLine+itemDisable,
        nomStr[3],
        15,
        0,
        NULL
    };
    static ItemTemplate nom5 = {
        55,
        {85, 25, 98, 172},
        editLine+itemDisable,
        nomStr[4],
        15,
        0,
        NULL
    };
    static ItemTemplate nom6 = {
        56,
        {101, 25, 114, 172},
        editLine+itemDisable,
        nomStr[5],
        15,
        0,
        NULL
    };
    static ItemTemplate nomDesJoueurs = {
        67,
        {6, 24, 20, 223},
        longStatText2+itemDisable,
        "\1J\1\0\1S\1\0Nom des joueurs",
        23,
        0,
        NULL
    };
    static DialogTemplate nomsTemp = {
        {33, 36, 181, 283},
        TRUE,
        0L, {
            &OK,
            &annuler,
            &nom6,
            &nom5,
            &nom4,
            &nom3,
            &nom2,
            &nom1,
            &nomDesJoueurs,
            NULL
        }
    };
    GrafPortPtr dlgJoueurs;
    Word i, j, itemHit;

    for (i = 0; i < 6; i++)
        memcpy(nomStr[i], pJeu.joueur[i].nom, LNOM);
    dlgJoueurs = GetNewModalDialog(&nomsTemp);
    SelectIText(dlgJoueurs, 51, 0, 0xffff);
    do
        itemHit = ModalDialog(0L);
    while (itemHit != 1 && itemHit != 2);
    if (itemHit == 1) {
        for (i = 0; i < 6; i++) {
            GetIText(dlgJoueurs, 51+i, pJeu.joueur[i].nom);
            pJeu.joueur[i].enJeu = pJeu.joueur[i].nom[0] ? TRUE : FALSE;
        }
        if (!pJeu.joueur[pJeu.tour].enJeu)
            JoueurSuivant();
    }
    RedessineJFen();
    CloseDialog(dlgJoueurs);
    jeu.action = TRUE;
}

void DoScores()
{
    GrafPortPtr scoresDlg, oldPort;
    char buf[6];
    Word i, j, tEssai, tSucces;
    static ItemTemplate iOK = {
        ok,
        { 122, 107, 137, 157 },
        buttonItem,
        "\pOK",
        0,
        simpSquare,
        NIL
    };
    static char lScores[] = {
        0x01, 'J', 0x01, 0x00,
        0x01, 'S', 0x01, 0x00,
        'S', 'c', 'o', 'r', 'e', 's'
    };
    static ItemTemplate iScores = {
        3,
        { 4, 85, 16, 180 },
        longStatText2+itemDisable,
        lScores,
        sizeof(lScores),
        0,
        NULL
    };
    static ItemTemplate iJoueurs = {
        4,
        { 20, 5, 32, 55 },
        statText+itemDisable,
        "\pJoueur",
        0,
        0,
        NULL
    };
    static ItemTemplate iThemes = {
        5,
        { 20, 109, 32, 159 },
        statText+itemDisable,
        "\pTh\217mes",
        0,
        0,
        NULL
    };
    static char lNumThemes[] = {
        0x01, 'C', 0x01, 0x00,
        '1', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x02, 0x00,
        '2', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x03, 0x00,
        '3', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x04, 0x00,
        '4', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x05, 0x00,
        '5', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x06, 0x00,
        '6', ' ', ' ', ' ', ' ',
        0x01, 'C', 0x00, 0x00,
        '%'
    };
    static ItemTemplate iNumThemes = {
        6,
        { 31, 110, 42, 265 },
        longStatText2+itemDisable,
        lNumThemes,
        sizeof(lNumThemes),
        0,
        NULL
    };
    static DialogTemplate tScores = {
        { 36, 27, 179, 292 },
        TRUE,
        0L, {
            &iOK,
            &iNumThemes,
            &iThemes,
            &iJoueurs,
            &iScores,
            NULL
        }
    };

    scoresDlg = GetNewModalDialog(&tScores);
    oldPort = GetPort();
    SetPort(scoresDlg);
    for (i = 0; i < 6 ; i++) {
        MoveTo(5, 50+12*i);
        DrawString(pJeu.joueur[i].nom);
        for (j = tEssai = tSucces = 0; j < 6 && pJeu.joueur[i].nom[0]; j++) {
            sprintf(buf, "%u", pJeu.joueur[i].total[j].essai);
            MoveTo(118+24*j-CStringWidth(buf), 50+12*i);
            DrawCString(buf);
            tEssai += pJeu.joueur[i].total[j].essai;
            tSucces += pJeu.joueur[i].total[j].succes;
        }
        if (pJeu.joueur[i].nom[0]) {
                sprintf(buf, "%u", tEssai ? (tSucces*100)/tEssai : 0);
                MoveTo(262-CStringWidth(buf), 50+12*i);
                DrawCString(buf);
        }
    }
    ModalDialog(0L);
    CloseDialog(scoresDlg);
    SetPort(oldPort);
}

void DoInfos()
{
    GrafPortPtr infosDlg, oldPort;
    Word i;
    char mask[8];

    infosDlg = GetNewModalDialog(&infosTemp);
    oldPort = GetPort();
    SetPort(infosDlg);
    MoveTo(57, 30);
    DrawCString(InfoRec.nom);
    sprintf(mask, "%.1d.%.1d", InfoRec.version/256, InfoRec.version & 0x00ff);
    MoveTo(86, 43);
    DrawCString(mask);
    sprintf(mask, "%d", InfoRec.nbQuest);
    MoveTo(175, 56);
    DrawCString(mask);
    for (i = 0; i < 6; i++) {
        SetForeColor(i+1);
        MoveTo(55, 85+11*i);
        DrawCString(InfoRec.theme[i]);
    }
    ModalDialog(0L);
    CloseDialog(infosDlg);
    SetPort(oldPort);
}

void Dialogue6Camemberts()
{
    AlertWindow(0, NULL, "90/\1J\1\0\r\r\1S\11\0Pas mal, pas mal...\r\r\
\1S\0\0Rendez-vous \210 la case centrale !/^#6");
}

void DialogueFinal()
{
    AlertWindow(0, NULL, "90/\1J\1\0\r\r\1S\11\0F\216licitations !\r\r\r\
\1S\0\0Vous avez brillamment gagn\216 les six camemberts, \
mais saurez-vous r\216pondre correctement \210 cette (derni\217re ?) \
question ?/^#6");
}

void DialogueBravo()
{
    AlertWindow(0, NULL, "90/\1J\1\0\1S\11\0\1C\6\0Bravo !\r\r\
\1S\0\0\1C\0\0Vous avez gagn\216 !!! Vous connaissez certainement les \
questions par c\317ur; aussi, n'attendez plus pour nous contacter afin de \
nous aider \210 taper les prochaines bases de questions Trivial Pursuit, \
et ainsi prendre part \210 cette \216pop\216e des temps modernes !\
\r\r3614 RTEL1 ou RTEL2 Bal Zubrowka/^#0");
}

void AfficheTheme(theme)
Word theme;
{
}

Word ChoixTheme()
{
    static StringHandle themeLibel[6] = {
        &themeBut1,
        &themeBut2,
        &themeBut3,
        &themeBut4,
        &themeBut5,
        &themeBut6
    };
    GrafPortPtr themeDlg;
    Word i, t;

    for (i = 0; i < 6; i++) {
        if (!(*themeLibel[i] = (StringPtr)malloc(sizeof(Str255))))
            SysErr();
        strcpy((*themeLibel[i])->text, InfoRec.theme[i]);
        (*themeLibel[i])->length = strlen((*themeLibel[i])->text);
    }
    themeDlg = GetNewModalDialog(&themeTemp);
    t = ModalDialog(0l);
    CloseDialog(themeDlg);
    for (i = 0; i < 6; i++)
        free(*themeLibel[i]);
    return t - 2;
}

Boolean DoQuestRep(theme)
Word theme;
{
    void Parle();

    GrafPortPtr dlgQR, oldPort;
    int ItemHit1, ItemHit2, pas, rc, rech;
    static char jReponse[256];
    static tQuestion Question;
    static char CarteID[6];
    static Rect r = { 131, 50, 145, 230 };

    static ItemTemplate bValid = {            
        1,             
        150, 14, 165, 110,      
        0x000A,         
        "\pValider",      
        0,       
        0x0003,     
        (void  *)NULL  
    };
    static ItemTemplate bSaisPas = {
        2,                   
        150, 134, 165, 282,      
        0x000A,          
        "\pJe ne sais pas...",      
        0,       
        0x0002,     
        (void  *)NULL  
    };
    static ItemTemplate bFaux = {            
        3,             
        150,134,165,281,       
        0x000A,          
        "\pJ'ai faux",      
        0,       
        0x0002,     
        (void  *)NULL 
    };
    static ItemTemplate bBon = {
        4,             
        150, 14,165,111,       
        0x000A,          
        "\pJ'ai bon",      
        0,       
        0x0002,     
        (void  *)NULL  
    };
    static ItemTemplate leReponse = {
        5,             
        115, 4, 130, 287,       
        editLine+itemDisable,          
        NULL,      
        40,      
        0x0000,     
        (void  *)NULL  
    };
    static ItemTemplate bCont = {
        6,             
        150, 100, 165, 190,      
        0x000A,         
        "\pContinuer",      
        0,       
        0x0003,     
        (void  *)NULL  
    };
    static ItemTemplate bVoir = {
        8,                   
        150, 134, 165, 282,      
        0x000A,          
        "\pVoir la R\216ponse...",      
        0,       
        0x0002,     
        (void  *)NULL  
    };
    static DialogTemplate tmpDlgQR = {
        { 21, 16, 192, 308 },       
        TRUE,     
        NULL,                    
        &leReponse,
        &bValid,   
        &bSaisPas,   
        &bVoir,
        &bBon,
        &bFaux,
        &bCont,  
        NULL
    };            

    Question.qSujet = theme;
    Question.qDiff = pJeu.diff; /*   Suivant la valeur du menu difficulte */
    if ((rc = nouvQuest(&Question)) == 0) {
         dlgQR = GetNewModalDialog(&tmpDlgQR);
         HideDItem(dlgQR, 3);
         HideDItem(dlgQR, 4);
         HideDItem(dlgQR, 6);
         HideDItem(dlgQR, 8);
         oldPort = GetPort();
         SetPort(dlgQR);
         MoveTo(10, 10);
         InstallFont(8*256+boldMask, shaston, 0);
         SetForeColor(Question.qSujet);
         DrawCString(InfoRec.theme[Question.qSujet - 1]);
         SetForeColor(0);

/* Pour la version de test, a supprimer par la suite*/
         InstallFont(9*256+0, times, 0);
         sprintf(CarteID, "%u", Question.qCarte);
         MoveTo(270, 10);
         DrawCString(CarteID);
/*   Fin du bloc a supprimer */

         InstallFont(12*256+0, geneva, 0);
         pas = Ecrit(Question.qQuestion, 30);
         InstallFont(8*256+0, shaston, 0);
         Parle(Question.qQuestion);
/* Attente Reponse */
         do
             if ((ItemHit1 = ModalDialog(0L)) == 1) {
                GetIText(dlgQR, 5, jReponse);
                if (jReponse[0])
                    switch (rech = Compare(Question.qReponse, jReponse)) {
                        case 0 :
                            MoveTo(50, 143);
                            DrawCString("R\216ponse \210 priori fausse...");
                            SelectIText(dlgQR, 5, 0, jReponse[0]);
                            HideDItem(dlgQR, 2);
                            ShowDItem(dlgQR, 8);
                            break;
                        case 1 :
                            HideDItem(dlgQR, 8);
                            ShowDItem(dlgQR, 3);
                            ShowDItem(dlgQR, 4);
                            break;
                        case 2 :
                            MoveTo(50, 143);
                            if (sonActif)
                                JoueSon(&yeah, 200, 0);
                            DrawCString("Bravo !!! R\216ponse exacte !!!");
                    }
             }
         while (ItemHit1 == 1 && (!jReponse[0] || !rech));
         RemoveDItem(dlgQR, 1);
         HideDItem(dlgQR, 2);
         if (ItemHit1 == 2 || rech == 2) {
              RemoveDItem(dlgQR, 5);
              HideDItem(dlgQR, 8);
              ShowDItem(dlgQR, 6);
              SetDefButton(6, dlgQR);
         } else {
              EraseRect(&r);
              HideDItem(dlgQR, 8);
              ShowDItem(dlgQR, 3);
              ShowDItem(dlgQR, 4);
              SelectIText(dlgQR, 5, 0, jReponse[0]);
         }
         InstallFont(12*256+0, geneva, 0);
         Ecrit(Question.qReponse, pas + 10);
         Parle(Question.qReponse);
         InstallFont(8*256+0, shaston, 0);
         ItemHit2 = ModalDialog(0L);
         CloseDialog(dlgQR);
         SetPort(oldPort);
         return (ItemHit2 == 4 || (ItemHit2 == 6 && ItemHit1 == 1)) ?
              TRUE : FALSE;
    } else
        switch (rc) {
            case -2 :
            case -3 : AlertWindow(0, NULL,
"33/\
Questions \216puis\216es; la base va \220tre r\216initialis\216e./\
^#6\0");
                      reInitBase();
                      break;
            default : SysErr();
        }
}

Word Compare(qRep, jRep)
char *qRep;
char *jRep;
{
     int i=0;
     char cqRep[255];
     char comparaison[5];

     p2cstr(jRep);

     Majuscule(cqRep, qRep);
     Majuscule(jRep, jRep);

     if (strcmp(cqRep, jRep) == 0)
          return 2;
     else
          for (i=0; jRep[i]; i++)
               if (strnstr(cqRep, jRep+i, 4))
                    return 1;
     return 0;
}

void Majuscule(dest, chaine)
char *dest, *chaine;
{
        do {
                switch (*chaine) {
                        case '"':
                        case '<':
                        case '\47':
                        case '.':
                        case '-':
                        case ',':
                        case ';':
                                *dest++=' ';
                                break;
                        case 142:
                        case 143:
                        case 144:
                        case 145:
                                *dest++='E';
                                break;
                        case 136:
                        case 137:
                                *dest++='A';
                                break;
                        case 148:
                        case 149:
                                *dest++='I';
                                break;
                        case 153:
                                *dest++='O';
                                break;
                        case 141:
                                *dest++='C';
                                break;
                        case 157:
                        case 158:
                                *dest++='U';
                                break;
                        default:
                                *dest++=toupper(*chaine);
                                break;
                }
        } while (*chaine++);
}

char *strnstr(src, sub, n)
char *src, *sub;
Word n;
{
     char *p1, *p2;
     Word i;

     while (*src) {
          if (*src == *sub) {
               p1 = src;
               p2 = sub;
               for (i = 0; i < n && *p1 && *p2 && *p1 == *p2; i++) {
                    p1++;
                    p2++;
               }
               if (i == n)
                    return src;
          }
          src++;
     }
     return NULL;
}

void DoEdition()
{
        AlertWindow(0, NULL, "90/\
\1J\1\0\Si vous d\216sirez participer \210 \15\
l'\216laboration de la prochaine Base \15\
\de Questions de \15\
\1F\5\0\000\014Trivial Pursuit GS\15\1F\0\0\0\0 \r \
Faites vous conna\224tre !!! \15\
Minitel : 3614 RTEL1 ou RTEL2 \15\
Bal Zubrowka/^#6\0");
}

int Ecrit(p, v)
char *p;
int v;
{
    int i, npix;
    Boolean lf = FALSE, italique = FALSE, guillemet = FALSE;

    MoveTo(10, v);
    npix = 272;
    do {
        for (i = 0; TextWidth(p, i+1) < npix && p[i] != '<' && p[i]; i++)
            if (p[i] == '"')
                p[i] = (guillemet = !guillemet) ? 0xc7 : 0xc8;
        if (TextWidth(p, i+1) >= npix)
            lf = TRUE;
        while (p[i] && (p[i] != ' ' || ispunct(p[i+1]) && p[i+1] != '"')
            && p[i-1] != '-' && p[i] != '<' && i) i--;
        if (i || p[i] == '\0' || p[i] == '<') {
            DrawText(p, i);
            if (p[i] == '\0')
                break;
            if (p[i] == '<')
                SetTextFace((italique = !italique) ? italicMask : 0);
            npix -= TextWidth(p, i + (p[i] == '<' ? 0 : 1));
            p += i + (p[i] == '<' || i && p[i-1] != '-' ? 1 : 0);
        }
        if (lf) {
            MoveTo(10, v += 15);
            lf = FALSE;
            npix = 272;
        }
    } while (TRUE);
    return v+15;
}

void Fermer()
{
    switch (AlertWindow(0, NULL,
        "32/Voulez-vous sauver la partie avant de fermer ?/^#2/#3/#1")) {
        case 0 : DoSauver();
        case 1 : jeu.action = FALSE;
    }
}

/***************************************************************************
*                Fonction TextenPhoneme                                    *
*                                                                          *
* Traduit texte normal en phonemes et parle les phonemes                                        *
*                                                                          *
***************************************************************************/

void Parle(chaine)
char *chaine;
{
        char chaineMaj[255];
        Majuscule(chaineMaj, chaine);
        printf("%s\n", chaineMaj);

        chaineMaj[0] = (char)strlen(chaineMaj+1);
        SetSayGlobals(0,6,4,5,9);
        Say(chaineMaj);


}   /* fin fonction Parle()  */
