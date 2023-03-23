*****************************************************************
*                                                               *
*     Startup segment de Trivial Pursuit                        *
*                                                               *
*     Affiche l'image TRIVIAL.PIC                               *
*                                                               *
*     Version 0.2    13/11/89  (Ajout decompactage de l'image)  *
*                                                               *
*****************************************************************

               keep  startup
               mcopy 3/src/startup.mac

Init           start startup
               kind  $10                segment startup
               using Donnees

               phk
               plb
               short m
               lda   #$41
               sta   >$C029
               long  m
               pea   0
               ph4   #Image
               ph2   L_Image
               ph4   #PicPtr
               ph4   #PicSize
               _UnPackBytes
               bcs   Fin
               pla
               short m
               lda   #$C1
               sta   >$C029
               long  m
Fin            rtl

PicPtr         dc    i4'$E12000'
PicSize        dc    i2'$8000'

               end


Donnees        data  startup
Image          anop
               copy  3/src/trivial.pak.1
L_Image        dc    i2'L_Image-Image'
               end
