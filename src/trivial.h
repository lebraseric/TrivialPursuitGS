/*
 *  trivial.h -- Header de l'application
 */

#ifndef __types__ 
#include <TYPES.H> 
#endif 


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


/*  Definitions de types  */

typedef struct {
   Word qCarte, qSujet, qDiff;
   char qQuestion[256], qReponse[256];
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


/*  Fonctions de main.1  */

int main();
void ActivationFen();
void ByeBye();
void InitFont();
void ExecMenu();
void DoQuitter();
void DoNouveau();
void DoSetDiff();
void Coche();
void DoActiveSon();
void PlaceMenus();
void DecompacteFond();
void ColorCycle();
void JoueSon();


/*  Fonctions de dialogs.1  */

int OuBase();
void DoAbout();
void DoOuvrir();
void DoSauver();
void DoSauverSous();
void DoInstr();
Word PageLen();
void InstrPage();
void DoNoms();
void DoScores();
void DoInfos();
void Dialogue6Camemberts();
void DialogueFinal();
void DialogueBravo();
Word ChoixTheme();
Boolean DoQuestRep();
Word Compare();
void Majuscule();
char *strnstr();
int Ecrit();
void Fermer();


/*  Fonctions de desk.1  */

void ClickBureau();
void Question();
Word TrouveTheme();
void Chemins();
void Explore();
void RedessineFond();
pascal void DessineFond();


/*  Fonctions de jfen.1  */

void InitJoueursDialog();
void DoModelessEvent();
void LanceDe();
void RedessineJFen();
void ContenuJFen();
pascal void DessineParts();
pascal void DessineDe();
void JoueurSuivant();


/*  Fonctions de os.1  */

int initBase(/* char *cheminBase, tInfoRec *infoRec */);
void reInitBase();
int nouvQuest(/* tQuestion *question */);
int fermeBase();
int decode(/* tQuestion *question, long loc, FILE *fic */);
void litchamps(/* char *dest */);
char litc();
char nextc();
void Ouvrir();
void Enregistrer();
