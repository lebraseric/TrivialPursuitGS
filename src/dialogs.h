/*
 *  dialogs.1 -- Gestion des dialogues de Trivial Pursuit
 */

#ifndef DIALOGS_H
#define DIALOGS_H

#include <types.h>

pascal int filterBase(long);
pascal int filterOuvrir(long);
int OuBase(void);
void DoOuvrir(void);
void DoSauver(void);
void DoSauverSous(void);
void DoInstr(void);
Word PageLen(char *pg);
void InstrPage(int npage);
void DoAbout(void);
void DoNoms(void);
void DoScores(void);
void DoInfos(void);
void Dialogue6Camemberts(void);
void DialogueFinal(void);
void DialogueBravo(void);
void AfficheTheme(Word theme);
Word ChoixTheme(void);
Boolean DoQuestRep(Word theme);
Word Compare(char *qRep, char *jRep);
void Majuscule(char *dest, char *chaine);
char *strnstr(char *src, char *sub, Word n);
void DoEdition(void);
int Ecrit(char *p, int v);
void Fermer(void);

unsigned char select_file_is_folder(void);

#endif /* DIALOGS */