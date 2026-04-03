*****************************************************************
*                                                               *
*    Definitions de dialogues                                   *
*                                                               *
*****************************************************************

           copy      2:ainclude:e16.control
           copy      2:ainclude:e16.dialog
           case   on

triviaSourceTmpl    data    dlgdata

                    dc    i'21,27,194,292'
                    dc    i'$FFFF'
                    dc    i4'0'
                    dc    a4'Item01'
                    dc    i4'0'

Item01              dc    i'1'
                    dc    i'154,108,168,155'
                    dc    i'buttonItem'
                    dc    a4'tpOK'
                    dc    i'0'
                    dc    i'simpSquare'
                    dc    i4'0'                    
                    
                    end

infosTmpl      data  dlgdata

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
               dc    i'buttonItem'
               dc    a4'tpOK'
               dc    i'0'
               dc    i'simpSquare'
               dc    i4'0'

Item167        dc    i'67'
               dc    i'5,84,17,179'
               dc    i'longStatText2+itemDisable'
               dc    i4'tBaseInfo'
               dc    i2'tBaseInfo_end-tBaseInfo'
               dc    i'0'
               dc    i4'0'

Item168        dc    i'68'
               dc    i'22,21,34,55'
               dc    i'longStatText2+itemDisable'
               dc    i4'tName'
               dc    i'tName_end-tName'
               dc    i'0'
               dc    i4'0'

Item169        dc    i'69'
               dc    i'35,21,47,84'
               dc    i'longStatText2+itemDisable'
               dc    i4'tVersion'
               dc    i'tVersion_end-tVersion'
               dc    i'0'
               dc    i4'0'

Item170        dc    i'70'
               dc    i'48,21,60,171'
               dc    i'longStatText2+itemDisable'
               dc    i4'tQuestionNb'
               dc    i'tQuestionNb_end-tQuestionNb'
               dc    i'0'
               dc    i4'0'

Item171        dc    i'71'
               dc    i'65,8,77,103'
               dc    i'longStatText2+itemDisable'
               dc    i4'tCategories'
               dc    i'tCategories_end-tCategories'
               dc    i'0'
               dc    i4'0'

               end


themeTmpl      data  dlgdata

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
               dc    i'buttonItem'
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
               dc    i'buttonItem'
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
               dc    i'buttonItem'
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
               dc    i'buttonItem'
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
               dc    i'buttonItem'
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
               dc    i'buttonItem'
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
               dc    i'longStatText2+itemDisable'
               dc    i4'tCatChoose'
               dc    i'tCatChoose_end-tCatChoose'
               dc    i'0'
               dc    i4'0'

               end
