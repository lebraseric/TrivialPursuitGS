/*
MERGE

Fusion des fichiers .INFO,  .IND et .DATA afin de produire une base de
questions Trivial Pursuit.


Syntaxe :

MERGE base

base : nom generique de la base de questions Trivial Pursuit


Auteur : Eric Le Bras


Date : 10 Octobre 1989

*/

#include <STDIO.H>
#include <ERRNO.H>
#include <STRING.H>
#include <TYPES.H>
#include "GLOBAL.H"

#define LBUFF 1024

FILE *fbase, *finfo, *findex, *fdata;
tInfoRec infoRec;
char buffer[LBUFF];
char *msg[] = {
   "\nSyntaxe : MERGE base",
   "Erreur n[",
   "Fichier non trouv{",
   "Nom de base > 10 caract}res",
   "Nombre de questions erron{"
};

void merge();
void openfiles();
void errfic();
void writeinfo();
char *convstr();
void writeindex();
void writedata();
void erreur();
void errnum();

main(argc, argv)
int argc;
char **argv;
{
   if (argc != 2) erreur(0);
   printf("GENERATION DE BASES DE QUESTIONS TRIVIAL PURSUIT\n");
   printf("Version 0.1    10/10/89\n\n");
   merge(*(argv+1));
}

void merge(base)
char *base;
{
   openfiles(base);
   printf("G{n{ration de la base %s\n", base);
   writeinfo();
   writeindex();
   writedata();
   fclose(finfo);
   fclose(findex);
   fclose(fdata);
   fclose(fbase);
}

void openfiles(base)
char *base;
{
   char nomfic[16];
   word l;

   if ((l = strlen(base)) > 10) erreur(3);
   strcpy(nomfic, base);
   strcpy(nomfic + l, ".info");
   if (!(finfo = fopen(nomfic, "r")))
       if (errno == ENOENT)
           errfic(nomfic);
       else
           errnum();
   strcpy(nomfic + l, ".ind");
   if (!(findex = fopen(nomfic, "rb")))
       if (errno == ENOENT)
           errfic(nomfic);
       else
           errnum();
   strcpy(nomfic + l, ".data");
   if (!(fdata = fopen(nomfic, "rb")))
       if (errno == ENOENT)
           errfic(nomfic);
       else
           errnum();
   if (!(fbase = fopen(base, "wb"))) errnum();
}

void errfic(nomfic)
char *nomfic;
{
   printf("%s : %s\n", *(msg+2), nomfic);
   exit(2);
}

void writeinfo()
{
   word linfo;

   linfo = sizeof(infoRec);
   if (!fwrite((char *)&linfo, sizeof(linfo), 1, fbase)) errnum();
   if (!fgets(buffer, LBUFF, finfo) && !feof(finfo)) errnum();
   while (!feof(finfo)) {
       switch (toupper(*buffer)) {
           case 'N' : sscanf(buffer, "%*c:%29[^\n]", infoRec.nom);
                      break;
           case 'V' : sscanf(buffer, "%*c:%u", &infoRec.version);
                      break;
           case '#' : sscanf(buffer, "%*c:%u", &infoRec.nbQuest);
                      break;
           case '1' :
           case '2' :
           case '3' :
           case '4' :
           case '5' :
           case '6' : convstr(infoRec.theme[*buffer - 49], buffer + 2);
                      break;
       }
       if (!fgets(buffer, LBUFF, finfo) && !feof(finfo)) errnum();
   }
   if (!fwrite((char *)&infoRec, sizeof(infoRec), 1, fbase)) errnum();
}

char *convstr(dest, src)
char *dest, *src;
{
   char *p = dest;

   while (*src != '\n') {
       switch (*src) {
           case '@' : *dest = '\210';
                      break;
           case '\\' : *dest = '\215';
                       break;
           case '{' : *dest = '\216';
                      break;
           case '}' : *dest = '\217';
                      break;
           case '|' : *dest = '\235';
                      break;
           case '~' :
           case '^' : switch (*(src+1)) {
                           case 'a' : *dest = '\211';
                                      break;
                           case 'e' : *dest = '\220';
                                      break;
                           case 'i' : *dest = '\224';
                                      break;
                           case 'o' : *dest = '\231';
                                      break;
                           case 'u' : *dest = '\236';
                                      break;
                           default : *dest = *(src+1);
                      }
                      if (*(src+1) != '\n')
                           if (*src++ == '~') (*dest)++;
                      break;
           default : *dest = *src;
       }
       dest++;
       src++;
   }
   *dest = '\0';
   return p;
}

void writeindex()
{
   word offset, i;
   struct {
       char filler[3];
       long adrQuest;
   } indrec;

   if (!fread((char *)&offset, sizeof(offset), 1, findex)) errnum();
   if (offset/sizeof(indrec) != infoRec.nbQuest) erreur(4);
   offset += sizeof(infoRec) + 4;
   if (!fwrite((char *)&offset, sizeof(offset), 1, fbase)) errnum();
   for (i=1; i<=infoRec.nbQuest; i++) {
       if (!fread(&indrec, sizeof(indrec), 1, findex)) errnum();
       indrec.adrQuest += offset;
       if (!fwrite(&indrec, sizeof(indrec), 1, fbase)) errnum();
   }
}

void writedata()
{
   word ldata;

   if (!fread((char *)&ldata, sizeof(ldata), 1, fdata)) errnum();
   while (!feof(fdata)) {
       if (!fread(buffer, 1, ldata, fdata)) errnum();
       if (!fwrite((char *)&ldata, sizeof(ldata), 1, fbase)) errnum();
       if (!fwrite(buffer, 1, ldata, fbase)) errnum();
       if (!fread((char *)&ldata, sizeof(ldata), 1, fdata) && !feof(fdata))
           errnum();
   }
}

void erreur(n)
word n;
{
   printf("%s\n", *(msg+n));
   exit(n);
}

void errnum()
{
   printf("%s %d\n", *(msg+1), errno);
   exit(1);
}
