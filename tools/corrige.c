/****************************************************************

Correction automatique de lots de questions Trivial Pursuit.
Le lot corrige est envoye sur la sortie standard.


Syntaxe :

corrige -v|-t [lot]

-v  : Affichage de la version
-t  : Tri des questions par no de carte et de theme
lot : Chemin d'acces (contenant le nom de fichier) du lot a corriger.
      Si le parametre lot n'est pas specifie, l'entree est prise sur l'entree
      standard.

Auteur : Eric LE BRAS


Versions :
0.1    03/01/90    Tri des questions

****************************************************************/

#include <stdio.h>
#include <string.h>
#include <types.h>

#define LBUFF   256

FILE *lot;
char buffer[LBUFF];
struct elemtable {
    union {
         struct {
              Word numquest;
              Word numcarte;
         } detail;
         Longword cle;
    } ucle;
    Word diff;
    long offset;
} table[1000];

int main();
void corrige();
void litligne();
void strupr();
void error();

main(argc, argv)
int argc;
char *argv[];
{
    void fermelot();
    Boolean erreur = FALSE;

    if (argc > 1)
         if (!strcmp(argv[1], "-v")) {
              printf("corrige v0.1  3/1/90\n");
              printf("Correction de lots de questions Trivial Pursuit\n");
              printf("Ecrit par Eric LE BRAS\n");
         } else if (!strcmp(argv[1], "-t"))
              if (argc == 2) /* pas de specification du fichier en entree */
                   corrige(argv[1], stdin);
              else
                   if ((lot = fopen(argv[2], "r")) == NULL)
                        error("corrige: impossible d'ouvrir %s", argv[2]);
                   else {
                        corrige(argv[1], lot);
                        onexit(fermelot);
                   }
         else /* parametre d'action incorrect */
              erreur = TRUE;
    else /* pas d'argument */
         erreur = TRUE;
    if (erreur)
         error("utilisation: corrige -v|-t [lot] [>sortie]", NULL);
}

void corrige(action, lot)
char *action;
FILE *lot;
{
    int n, i = 0;
    int compare();

    litligne();
    strupr(buffer);
    if (strcmp(buffer, "GENUS\n") || feof(lot))
         error("corrige: pas un lot de questions GENUS", NULL);
    fprintf(stderr, "corrige: lecture des questions\n");
    for (litligne(); !feof(lot); litligne()) {
         if (*buffer == '#') {
              sscanf(buffer, "#%u,%u,%u",
                   &table[n].ucle.detail.numcarte,
                   &table[n].ucle.detail.numquest,
                   &table[n].diff);
              table[n++].offset = ftell(lot);
         }
    }
    fprintf(stderr, "corrige: tri\n");
    qsort(table, n, sizeof(*table), compare);
    fprintf(stderr, "corrige: copie du fichier trie\n");
    printf("GENUS\n");
    for (i = 0; i < n; i++) {
         printf("#%u,%u,%u\n",
              table[i].ucle.detail.numcarte,
              table[i].ucle.detail.numquest,
              table[i].diff);
         fseek(lot, table[i].offset, 0);
         for (litligne(); *buffer != '#' && !feof(lot); litligne())
              fputs(buffer, stdout);
    }
}

int compare(q1, q2)
struct elemtable *q1, *q2;
{
    long d;

    d = q1->ucle.cle - q2->ucle.cle;
    return d ? (d > 0 ? 1 : -1) : 0;
}

void litligne()
{
    if (fgets(buffer, LBUFF, lot) == NULL && !feof(lot))
         error("corrige: erreur d'acces au fichier en entree", NULL);
}

void strupr(s)
char *s;
{
    while (*s++ = toupper(*s));
}

void fermelot()
{
    fclose(lot);
}

void error(s1, s2)
char *s1, *s2;
{
    fprintf(stderr, s1, s2);
    fprintf(stderr, "\n");
    exit(1);
}
