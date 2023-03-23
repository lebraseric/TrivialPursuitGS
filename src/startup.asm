*****************************************************************
*                                                               *
*     Startup segment de Trivial Pursuit                        *
*                                                               *
*     Affiche l'image TRIVIAL.PIC                               *
*                                                               *
*     Version 0.2    13/11/89  (Ajout decompactage de l'image)  *
*     Version 0.3    23/03/23  (Renommage macros AppleUtil)     *
*                                                               *
*****************************************************************

               keep  startup
               mcopy macros/startup.mac

Init           start startup
               kind  $10                segment startup
               using Donnees

               phk
               plb
               shortm
               lda   #$41
               sta   >$C029
               longm
               pea   0
               pushlong   #Image
               pushword   L_Image
               pushlong   #PicPtr
               pushlong   #PicSize
               _UnPackBytes
               bcs   Fin
               pla
               shortm
               lda   #$C1
               sta   >$C029
               longm
Fin            rtl

PicPtr         dc    i4'$E12000'
PicSize        dc    i2'$8000'

               end


Donnees        data  startup
Image          anop
               copy  src/trivial.pak.asm
L_Image        dc    i2'L_Image-Image'
               end
