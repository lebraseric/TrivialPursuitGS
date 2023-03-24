/*
 *  trivial.h -- Header de l'application
 */

#ifndef TRIVIAL_H
#define TRIVIAL_H

#include "typedefs.h"
#include <qdaux.h>
#include <dialog.h>
#include <types.h> 


extern int _toolErr;


// Screen background

extern DataBlock fondEcran;


// Icons

extern QDIconRecord homme1Icon;
extern QDIconRecord homme2Icon;
extern QDIconRecord homme3Icon;
extern QDIconRecord femme1Icon;
extern QDIconRecord femme2Icon;
extern QDIconRecord femme3Icon;
extern QDIconRecord pion1Icon;
extern QDIconRecord pion2Icon;
extern QDIconRecord pion3Icon;
extern QDIconRecord pion4Icon;
extern QDIconRecord pion5Icon;
extern QDIconRecord pion6Icon;
extern QDIconRecord pionCentreIcon;
extern QDIconRecord pionDeIcon;
extern QDIconRecord partIcon;
extern QDIconRecord Un, Deux, Trois, Quatre, Cinq, Six, Cliquer;
extern QDIconRecord camembertIcon;


// Dialogs data

extern DialogTemplate infosTemp;
extern DialogTemplate themeTemp;
extern StringPtr themeBut1,themeBut2,themeBut3,themeBut4,themeBut5,themeBut6;


// Sounds

extern DataBlock dice;
extern DataBlock yeah;
extern DataBlock clap;
extern DataBlock decu;


// Globals

extern char titreActif[];
extern char titreInactif[];
extern Boolean sonActif;
extern WmTaskRec tache;
extern tInfoRec InfoRec;
extern Jeu jeu;
extern PJeu pJeu;
extern Ecran **imageHdl;


#endif /* TRIVIAL_H */
