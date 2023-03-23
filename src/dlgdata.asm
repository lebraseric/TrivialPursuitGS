*****************************************************************
*                                                               *
*    Definitions de dialogues                                   *
*                                                               *
*****************************************************************

           mcopy     3:src:dlgdata.mac
           copy      2:ainclude:e16.control
           objcase   on

InfosTemp      data  ~arrays

infosTemp      entry
               dc    i'21,27,194,292'
               dc    i'$FFFF'
               dc    i4'0'
               dc    i4'Item171'
               dc    i4'Item170'
               dc    i4'Item169'
               dc    i4'Item168'
               dc    i4'Item167'
               dc    i4'Item11'
               dc    i4'0'



Item11         dc    i'1'
               dc    i'154,108,168,155'
               dc    i'$000A'
               dc    i4'BTitle11'
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor11'
BTitle11       str   'OK'
BColor11       dc    i'$0000'
               dc    i'$00F0'
               dc    i'$0000'
               dc    i'$00F0'
               dc    i'$000F'


Item167        dc    i'67'
               dc    i'5,84,17,179'
               dc    i'$8016'
               dc    i4'TTitle167'
               dc    i'TTEnd167-TTitle167'
               dc    i'0'
               dc    i4'0'
TTitle167      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0001'
               dc    h'01',c'S',i'$0001'
               dc    c'Infos Base'
               dc    h'01',c'S',i'$0000'
TTEnd167       dc    i'0'


Item168        dc    i'68'
               dc    i'22,21,34,55'
               dc    i'$8016'
               dc    i4'TTitle168'
               dc    i'TTEnd168-TTitle168'
               dc    i'0'
               dc    i4'0'
TTitle168      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0000'
               dc    c'Nom :'
TTEnd168       dc    i'0'


Item169        dc    i'69'
               dc    i'35,21,47,84'
               dc    i'$8016'
               dc    i4'TTitle169'
               dc    i'TTEnd169-TTitle169'
               dc    i'0'
               dc    i4'0'
TTitle169      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0000'
               dc    c'Version :'
TTEnd169       dc    i'0'


Item170        dc    i'70'
               dc    i'48,21,60,171'
               dc    i'$8016'
               dc    i4'TTitle170'
               dc    i'TTEnd170-TTitle170'
               dc    i'0'
               dc    i4'0'
TTitle170      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0000'
               dc    c'Nombre de questions :'
TTEnd170       dc    i'0'


Item171        dc    i'71'
               dc    i'65,8,77,103'
               dc    i'$8016'
               dc    i4'TTitle171'
               dc    i'TTEnd171-TTitle171'
               dc    i'0'
               dc    i4'0'
TTitle171      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0000'
               dc    h'01',c'S',i'$0002'
               dc    c'Th',h'8F',c'mes :'
               dc    h'01',c'S',i'$0000'
TTEnd171       dc    i'0'

               end



ThemeTemp      data  ~arrays

themeTemp      entry
               dc    i'38,27,177,292'
               dc    i'$FFFF'
               dc    i4'0'
               dc    i4'Item267'
               dc    i4'Item26'
               dc    i4'Item25'
               dc    i4'Item24'
               dc    i4'Item23'
               dc    i4'Item22'
               dc    i4'Item21'
               dc    i4'0'



Item21         dc    i'2'
               dc    i'24,38,38,228'
               dc    i'$000A'
themeBut1      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor21'
BColor21       dc    i'$0000'
               dc    i'$0010'
               dc    i'$0000'
               dc    i'$0010'
               dc    i'$000F'


Item22         dc    i'3'
               dc    i'43,38,57,228'
               dc    i'$000A'
themeBut2      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor22'
BColor22       dc    i'$0000'
               dc    i'$0020'
               dc    i'$0000'
               dc    i'$0020'
               dc    i'$000F'


Item23         dc    i'4'
               dc    i'62,38,76,228'
               dc    i'$000A'
themeBut3      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor23'
BColor23       dc    i'$0000'
               dc    i'$0030'
               dc    i'$0000'
               dc    i'$0030'
               dc    i'$000F'


Item24         dc    i'5'
               dc    i'81,38,95,228'
               dc    i'$000A'
themeBut4      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor24'
BColor24       dc    i'$0000'
               dc    i'$0040'
               dc    i'$0000'
               dc    i'$0040'
               dc    i'$000F'


Item25         dc    i'6'
               dc    i'100,38,114,228'
               dc    i'$000A'
themeBut5      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor25'
BColor25       dc    i'$0000'
               dc    i'$0050'
               dc    i'$0000'
               dc    i'$0050'
               dc    i'$000F'


Item26         dc    i'7'
               dc    i'119,38,133,228'
               dc    i'$000A'
themeBut6      entry
               ds    4
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'BColor26'
BColor26       dc    i'$0000'
               dc    i'$0060'
               dc    i'$0000'
               dc    i'$0060'
               dc    i'$000F'


Item267        dc    i'67'
               dc    i'6,7,18,157'
               dc    i'$8016'
               dc    i4'TTitle267'
               dc    i'TTEnd267-TTitle267'
               dc    i'0'
               dc    i4'0'
TTitle267      anop
               dc    h'01',c'C',i'$0000'
               dc    h'01',c'B',i'$000F'
               dc    h'01',c'J',i'$0000'
               dc    c'Choisissez un th',h'8F',c'me :'
TTEnd267       dc    i'0'

               end
