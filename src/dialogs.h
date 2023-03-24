/*
 *  dialogs.1 -- Gestion des dialogues de Trivial Pursuit
 */

#ifndef DIALOGS_H
#define DIALOGS_H

#include <types.h>

int OuBase(void);
void DoOuvrir(void);
void DoSauver(void);
void DoSauverSous(void);
void DoInstr(void);
void DoAbout(void);
void DoNoms(void);
void DoScores(void);
void DoInfos(void);
void Dialogue6Camemberts(void);
void DialogueFinal(void);
void DialogueBravo(void);
Word ChoixTheme(void);
Boolean DoQuestRep(Word theme);
void DoEdition(void);
void Fermer(void);

unsigned char select_file_is_folder(void);

#endif /* DIALOGS */