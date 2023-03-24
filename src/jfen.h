/*
 *  Trivial Pursuit GS
 *
 *  jfen.h
 */

#ifndef JFEN_H
#define JFEN_H

#include <quickdraw.h>
#include <types.h>

extern GrafPortPtr joueursDialog;

void InitJoueursDialog(void);
void DoModelessEvent(void);
void LanceDe(void);
void RedessineJFen(void);
void ContenuJFen(void);
pascal void DessineParts(GrafPortPtr, Word);
pascal void DessineDe(GrafPortPtr, Word);
void JoueurSuivant(void);

#endif /* JFEN_H */
