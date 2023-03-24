/*
 *  Trivial Pursuit GS
 *
 *  os.h
 */

#ifndef OS_H
#define OS_H

int initBase(char *, tInfoRec *);
void reInitBase(void);
int nouvQuest(tQuestion *question);
int fermeBase(void);
void Ouvrir(void);
void Enregistrer(void);

#endif