/*
CONTROL

Controle technique d'un lot de questions.


Syntaxe :

CONTROL lot

lot : nom du lot de questions a controler

Versions :
   0.1 : Premiere realisation (18/09/89)
   0.2 : Statistiques par type de question (23/09/89)

Auteur : Eric Le Bras

*/

#include <STDIO.H>
#include <TYPES.H>
#include <CTYPE.H>
#include <STRING.H>
#include <ERRNO.H>
#include <stdlib.h>

#define LBUFF 256

FILE *flot;
char buffer[LBUFF + 1];
char *msg[] =
        {
                "Syntaxe : CONTROL lot",
                "Erreur",
                "Ligne",
                "Lot non trouv{",
                "Pas un lot de questions GENUS",
                "pas d'identification de question (#...)",
                "erreur num{ro de question",
                "erreur niveau de difficult{",
                "rupture de s{quence",
                "erreur num{ro de carte",
                "manque question et/ou r{ponse"
        };
word ligne;
word carte, question, diff;
word total = 0;
word sdiff[6][3] = { {0,0,0}, {0,0,0}, {0,0,0} };
word ancCarte = 0;
boolean anom = FALSE;

boolean control(char *lot);
void controlCarte(void);
void controlQuestion(word i);
void testeQuestion(void);
void analyseIdent(void);
void warn(word n);
void manqueQuestion(word n);
void questionEnTrop(word n);
void litLigne(char *buffer, int l, FILE *stream);
void prLigne(char *buffer);
void prStat(void);
void strupr(char *s);
void erreur(word n);
void errnum(int n);

int main(int argc, char **argv)
{
        if (argc != 2) erreur(0);
        exit(control(*(argv+1)));
}

boolean control(char *lot)
{
        if (!(flot = fopen(lot,"r")))
                if (errno == ENOENT)
                        erreur(3);
                else
                        errnum(errno);
        if (!fgets(buffer, LBUFF, flot)) errnum(errno);
        ligne = 1;
        strupr(buffer);
        if (strcmp(buffer, "GENUS\n")) erreur(4);
        printf("CONTROLE DE LOTS DE QUESTIONS TRIVIAL PURSUIT\n");
        printf("Version 0.2    23/09/89\n\n");
        printf("Controle du lot %s\n\n", lot);
        litLigne(buffer, LBUFF, flot);
        while (!feof(flot)) controlCarte();
        fclose(flot);
        prStat();
        return anom;
}

void controlCarte(void)
{
        word i;

        if (*buffer != '#') {
                warn(5);
                do
                        litLigne(buffer, LBUFF, flot);
                while (*buffer != '#' && !feof(flot));
        }
        else {
                if (carte>=1) {
                        if (ancCarte && carte != ancCarte+1) warn(8);
                        ancCarte = carte;
                        for (i=1; i<=6; controlQuestion(i++));
                }
                else
                        warn(9);
        }
}

void controlQuestion(word i)
{
        if (*buffer != '#') {
                warn(5);
                do
                        litLigne(buffer, LBUFF, flot);
                while (*buffer != '#' && !feof(flot));
        }
        if (carte != ancCarte)
                manqueQuestion(i);
        else {
                if (question < 1 || question > 6) warn(6);
                if (diff < 1 || diff > 3) warn(7);
                if (question != i)
                        if (question > i)
                                manqueQuestion(i);
                        else
                                questionEnTrop(i);
                if (question <= i) testeQuestion();
        }
}

void testeQuestion(void)
{
        litLigne(buffer, LBUFF, flot);
        if (*buffer == '#')
                warn(10);
        else {
                litLigne(buffer, LBUFF, flot);
                if (*buffer == '#')
                        warn(10);
                else
                        litLigne(buffer, LBUFF, flot);
        }
}

void analyseIdent(void)
{
        sscanf(buffer, "#%u,%u,%u", &carte, &question, &diff);
        total++;
        sdiff[question-1][diff-1]++;
}

void warn(word n)
{
        prLigne(buffer);
        printf("   -> %s\n", msg[n]);
}

void manqueQuestion(word n)
{
        prLigne(buffer);
        printf("   -> manque question n[%u\n", n);
}

void questionEnTrop(word n)
{
        prLigne(buffer);
        printf("   -> la question %u est en trop\n", n);
}

void litLigne(char *buffer, int l, FILE *stream)
{
        if (!fgets(buffer, l, stream) && !feof(flot)) errnum(errno);
        ligne++;
        if (*buffer == '#') analyseIdent();
}

void prLigne(char *buffer)
{
        char *p;

        p = buffer;
        while (*p++ = *p == '\n' ? 0 : *p);
        printf("%s %d : %.65s\n", msg[2], ligne, buffer);
        anom = TRUE;
}

void prStat(void)
{
        word i, j, tdiff[3];

        tdiff[0] = tdiff[1] = tdiff[2] = 0;
        printf("\nLe lot contient %u questions r{parties ainsi :\n\n", total);
        printf("Sujet  Diff. 1     Diff. 2     Diff.3");
        for (i=0; i<6; i++) {
                printf("\n%u      ", i+1);
                for (j=0; j<3; j++) {
                        printf("%4u (%2d%%)  ", sdiff[i][j],
                        100*sdiff[i][j]/(sdiff[i][0]+sdiff[i][1]+sdiff[i][2]));
                        tdiff[j] += sdiff[i][j];
                }
        }
        printf("\nTotal  ");
        for (j=0; j<3; j++)
                printf("%4u (%2d%%)  ", tdiff[j], 100*tdiff[j]/total);
        printf("\n");
}

void strupr(register char *s)
{
        while(*s++ = islower(*s) ? _toupper(*s) : *s);
}

void erreur(word n)
{
        printf("%s\n", *(msg+n));
        exit(n);
}

void errnum(int n)
{
        printf("%s %d\n", *(msg+1), n);
        exit(1);
}
