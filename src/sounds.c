/*
 *  Trivial Pursuit GS
 *
 *  Sound management
 */

#include <types.h>
#include <sound.h>
#include <menu.h>
#include "trivial.h"
#include "sounds.h"

void DoActiveSon(void)
{
         if (sonActif) {
                sonActif=FALSE;
                SetMItemName(titreInactif, 278);
         }
         else {
                sonActif=TRUE;
                SetMItemName(titreActif, 278);
        }
}

void JoueSon(DataBlock *son, Word vitesse, Word generateur)
{
        static SoundParamBlock sonParms;

        sonParms.waveStart = son->data;
        sonParms.waveSize = son->length/256;
        sonParms.freqOffset = vitesse;
        sonParms.docBuffer = 0;
        sonParms.bufferSize = 0;
        sonParms.volSetting = 250;
        sonParms.nextWavePtr = 0;
        FFStopSound(0x0001 << generateur);
        FFStartSound(0x0001 | generateur << 8 | generateur << 12, (Pointer)&sonParms);
}