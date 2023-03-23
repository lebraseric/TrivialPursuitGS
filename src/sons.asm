               objcase on

clap           data  gs
               dc    i2'fclap-dclap'
dclap          copy  src/clap.asm
fclap          anop
               end

dice           data  gs
               dc    i2'fdice-ddice'
ddice          copy  src/dice.asm
fdice          anop
               end

decu           data  gs
               dc    i2'fdecu-ddecu'
ddecu          copy  src/decu.asm
fdecu          anop
               end

yeah           data  gs
               dc    i2'fyeah-dyeah'
dyeah          copy  src/yeah.asm
fyeah          anop
               end
