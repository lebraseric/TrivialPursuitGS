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
    if (sonActif)
    {
        sonActif = FALSE;
        SetMItemName(titreInactif, 278);
    }
    else
    {
        sonActif = TRUE;
        SetMItemName(titreActif, 278);
    }
}

void JoueSon(DataBlock *sound, Word speed)
{
    static SoundParamBlock sonParms;
    Word i;

    sonParms.waveStart = sound->data;
    sonParms.waveSize = sound->length / 256;
    sonParms.freqOffset = speed;
    sonParms.docBuffer = 0;
    sonParms.bufferSize = 0;
    sonParms.volSetting = 250;
    sonParms.nextWavePtr = 0;
    for (i = 0; i < 15; i++)    // Loop if generator already in use
    {
        FFStopSound(0x0001 << i);
        FFStartSound(0x0001 | i << 8 | i << 12, (Pointer)&sonParms);
        if (_toolErr == 0)
            break;
    }
}