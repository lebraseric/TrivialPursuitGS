/*
APPEND

Ajout d'un lot de questions a une base de questions Trivial Pursuit.


Syntaxe :

APPEND base lot

base : nom generique de la base de questions trivial pursuit (longueur
       inferieure ou egale a 10 caracteres).
lot  : nom du lot de questions a ajouter a la base.


Auteur : Eric Le Bras


Versions :
0.1    03/09/89     Premiere realisation
0.3    17/10/89     Correction bug i accent circonflexe
0.4    31/12/89     Ajout du u trema

*/

#include <STDIO.H>
#include <TYPES.H>
#include <STRING.H>
#include <MEMORY.H>
#include <CTYPE.H>
#include <ERRNO.H>
#include <stdlib.h>

#define LBUFF 256

char index[16], data[16];
FILE *flot, *findex, *fdata;
char buffer[LBUFF + 1];
char dbuffer[1024];
char *destbuff;
Word lindex;
struct {
   char typeQuest;
   Word carte;
   long adrQuest;
   } indrec;
Word t = 2;
union {
    Word utw;
    char utc[2];
} ut;

char *msg[] =
   {
       "Syntaxe : APPEND base lot",
       "Nom de base > 10 caract}res",
       "Lot non trouv{",
       "Erreur syst}me",
       "Base non cr{{e",
       "Pas un lot de questions GENUS",
       "Caract}re non accentuable"
   };

void append(char *base, char *lot);
void openfiles(char *base, char *lot);
Boolean nouvbase(char *base);
void appendQuest(void);
void analyseIdent(void);
void code(char *orgbuff);
void addc(char c);
void erreur(Word n);
void errsys(void);
void errcar(int n, char c);
void strupr(char *s);

int main(int argc, char **argv)
{
   if (argc != 3) erreur(0);
   printf("CONCATENATION DE LOTS DE QUESTIONS TRIVIAL PURSUIT\n");
   printf("Version 0.4    31/12/89\n\n");
   append(*(argv+1), *(argv+2));
}

void append(char *base, char *lot)
{
   openfiles(base, lot);
   printf("Ajout du lot %s @ la base %s\n\n", lot, base);
   while (!feof(flot)) appendQuest();
   fclose(flot);
   fclose(fdata);
   rewind(findex);
   fwrite((char *)&lindex, sizeof(lindex), 1, findex);
   fclose(findex);
}

void openfiles(char *base, char *lot)
{
   if (strlen(base) > 10) erreur(1);
   strcpy(index, base);
   strcat(index, ".ind");
   strcpy(data, base);
   strcat(data, ".data");
   if (!(flot = fopen(lot,"r")))
       if (errno == ENOENT)
           erreur(2);
       else
           errsys();
   if (!fgets(buffer, LBUFF, flot)) errsys();
   strupr(buffer);
   if (strcmp(buffer, "GENUS\n")) erreur(5);
   if (!(findex = fopen(index,"rb+")))
       if (errno == ENOENT)
           if (nouvbase(base)) {
               lindex = 0;
               if (!(findex = fopen(index,"wb+"))) errsys();
               fwrite((char *)&lindex, sizeof(lindex), 1, findex);
           }
           else
               erreur(4);
       else
           errsys();
   else
       fread((char *)&lindex, sizeof(lindex), 1, findex);
       fseek(findex, 0L, 2);
   if (!(fdata = fopen(data,"ab"))) errsys();
   fseek(fdata, 0L, 2);
}

Boolean nouvbase(char *base)
{
   printf("Base %s inexistante; la cr{er ? (o/n) ", base);
   return (tolower(getchar()) == 'o');
}

void appendQuest(void)
{
   Word l;

   if (!fgets(buffer, LBUFF, flot) && !feof(flot)) errsys();
   if (!feof(flot)) {
       analyseIdent();
       if (!fwrite((char *)&indrec, sizeof(indrec), 1, findex)) errsys();
       lindex += sizeof(indrec);
       if (!fgets(buffer, LBUFF, flot)) errsys();
       destbuff = dbuffer;
       code(buffer);
       if (!fgets(buffer, LBUFF, flot)) errsys();
       code(buffer);
       while (t != 2) addc('\0');
       l = destbuff-dbuffer;
       if (!fwrite((char *)&l, sizeof(l), 1, fdata)) errsys();
       if (!fwrite(dbuffer, sizeof(*dbuffer), l, fdata)) errsys();
   }
}

void analyseIdent(void)
{
   Word question, diff;

   sscanf(buffer, "#%u,%u,%u", &indrec.carte, &question, &diff);
   indrec.typeQuest = (char)diff << 3 | (char)question;
   indrec.adrQuest = ftell(fdata);
}

void code(char *orgbuff)
{
   char accent;

   while (*orgbuff != '\n') {
       if (islower(*orgbuff) || *orgbuff == ' ' || *orgbuff == '@'
           || *orgbuff == '{' || *orgbuff == '}')
           if (islower(*orgbuff))
               addc(*orgbuff - 91);
           else
               switch (*orgbuff) {
                   case ' ' : addc('\002'); break;
                   case '@' : addc('\003'); break;
                   case '{' : addc('\004'); break;
                   case '}' : addc('\005'); break;
               }
       else {
           if (*orgbuff == '^' || *orgbuff == '~') {
               accent = *orgbuff;
               orgbuff++;
               if (accent == '^' && *orgbuff != 'a' && *orgbuff != 'e'
                   && *orgbuff != 'i' && *orgbuff != 'o' && *orgbuff != 'u')
                   errcar(6, *orgbuff);
               if (accent == '~' && *orgbuff != 'e' && *orgbuff != 'i'
                   && *orgbuff != 'u')
                   errcar(6, *orgbuff);
           }
           if (isupper(*orgbuff) || *orgbuff == 'e' || *orgbuff == 'i'
               || *orgbuff == 'u') {
               addc('\001');
               if (isupper(*orgbuff))
                   addc(*orgbuff - 59);
               else
                   switch (*orgbuff) {
                       case 'e' : if (accent == '^')
                                       addc('\000');
                                  else
                                       addc('\001');
                                  break;
                       case 'i' : if (accent == '^')
                                       addc('\002');
                                  else
                                       addc('\003');
                                  break;
                       case 'u' : if (accent == '^')
                                       addc('\004');
                                  else
                                       addc('\005');
                                  break;
                   }
           }
           else
               addc('\000');
               if (isdigit(*orgbuff))
                   addc(*orgbuff - 32);
               else
                   switch (*orgbuff) {
                       case '!' : addc('\001'); break;
                       case '"' : addc('\002'); break;
                       case 'a' : addc('\003'); break;
                       case '|' : addc('\004'); break;
                       case '%' : addc('\005'); break;
                       case '&' : addc('\006'); break;
                       case '\'' : addc('\007'); break;
                       case '(' : addc('\010'); break;
                       case ')' : addc('\011'); break;
                       case 'o' : addc('\012'); break;
                       case '+' : addc('\013'); break;
                       case ',' : addc('\014'); break;
                       case '-' : addc('\015'); break;
                       case '.' : addc('\016'); break;
                       case '\\' : addc('\017'); break;
                       case ':' : addc('\032'); break;
                       case ';' : addc('\033'); break;
                       case '<' : addc('\034'); break;
                       case '=' : addc('\035'); break;
                       case '[' : addc('\036'); break;
                       case '?' : addc('\037'); break;
                   }
       }
       orgbuff++;
   }
   addc('\000');
   addc('\000');
}

void addc(char c)
{
   ut.utw |= c << (5 * t + 1);
   if (t)
       t--;
   else {
       c = ut.utc[0];
       ut.utc[0] = ut.utc[1];
       ut.utc[1] = c;
       memcpy(destbuff, &ut, 2);
       destbuff += 2;
       ut.utw = 0;
       t = 2;
   }
}

void erreur(Word n)
{
   printf("%s\n", *(msg+n));
   exit(n);
}

void errsys(void)
{
   printf("%s %d\n", *(msg+3), errno);
   exit(1);
}

void errcar(int n, char c)
{
   printf("%s : %c\n", *(msg+n), c);
   exit(n);
}

void strupr(register char *s)
{
   while(*s++ = toupper(*s));
}
