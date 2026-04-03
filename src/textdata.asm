*****************************************************************
*                                                               *
*    Application text data                                      *
*                                                               *
*****************************************************************

                mcopy   macros/textdata.mac
                case    on

textdata        data    textdata

tpOK            entry
                str     'OK'
tpCancel        entry
                str     'Cancel'

; TriviaSourceTmpl strings

tTriviaSource       entry
                    dc      h'01',c'S',i'$0001',c'Trivia source'
tTriviaSource_end   entry

; InfosTmpl strings

tBaseInfo       entry
                dc      h'01',c'C',i'$0000'
                dc      h'01',c'B',i'$000F'
                dc      h'01',c'J',i'$0001'
                dc      h'01',c'S',i'$0001'
                dc      c'Infos Base'
                dc      h'01',c'S',i'$0000'
tBaseInfo_end   entry
tName           entry
                dc    h'01',c'C',i'$0000'
                dc    h'01',c'B',i'$000F'
                dc    h'01',c'J',i'$0000'
                dc    c'Nom :'
tName_end       entry
tVersion        entry
                dc    h'01',c'C',i'$0000'
                dc    h'01',c'B',i'$000F'
                dc    h'01',c'J',i'$0000'
                dc    c'Version :'
tVersion_end    entry
TTitle170      anop
tQuestionNb     entry
                dc    h'01',c'C',i'$0000'
                dc    h'01',c'B',i'$000F'
                dc    h'01',c'J',i'$0000'
                dc    c'Nombre de questions :'
tQuestionNb_end entry
tCategories     entry
                dc    h'01',c'C',i'$0000'
                dc    h'01',c'B',i'$000F'
                dc    h'01',c'J',i'$0000'
                dc    h'01',c'S',i'$0002'
                dc    c'Th',h'8F',c'mes :'
                dc    h'01',c'S',i'$0000'
tCategories_end entry

; ThemeTmpl strings

tCatChoose      entry
                dc    h'01',c'C',i'$0000'
                dc    h'01',c'B',i'$000F'
                dc    h'01',c'J',i'$0000'
                dc    c'Choisissez un th',h'8F',c'me :'
tCatChoose_end  entry

                end
