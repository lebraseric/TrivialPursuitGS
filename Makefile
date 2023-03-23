TARGETS = trivial

IIX = iix
CHTYP = iix chtyp
COMPILE = iix compile
ASSEMBLE = iix assemble
LINK = iix link
MACGEN = iix macgen

# CFLAGS = -I -P cc=-i"src" cc=-dFUJINET
CFLAGS = -I -P cc=-i"src"

OBJECTS = obj/startup.a obj/main.a obj/dialogs.a obj/desk.a \
	obj/jfen.a obj/os.a obj/initdesk.a obj/ticons.a obj/sons.a \
	obj/dlgdata.a

all : obj macros objects $(TARGETS)

.PHONY: clean
clean:
	rm $(TARGETS) obj/* macros/*
	rmdir obj macros

obj macros:
	mkdir $@

objects: $(OBJECTS)

trivial: $(OBJECTS)
	$(LINK) \
	obj/startup obj/main obj/dialogs obj/desk obj/jfen obj/os \
	obj/initdesk obj/ticons obj/sons obj/dlgdata \
	keep=$@

obj/%.ROOT: obj/%.A

macros/startup.mac: src/startup.asm
	$(MACGEN) src/startup.asm macros/startup.mac 2/ainclude/m16.= 2/appleutil/m16.Util2

obj/startup.a: src/startup.asm macros/startup.mac src/trivial.pak.asm
	$(ASSEMBLE) src/startup.asm keep=$*

obj/main.a: src/main.c src/trivial.h src/initdesk.h
	$(COMPILE) $(CFLAGS) src/main.c keep=$*

obj/dialogs.a: src/dialogs.c src/dialogs.h
	$(COMPILE) $(CFLAGS) src/dialogs.c keep=$*

obj/desk.a: src/desk.c src/trivial.h
	$(COMPILE) $(CFLAGS) src/desk.c keep=$*

obj/jfen.a: src/jfen.c src/trivial.h
	$(COMPILE) $(CFLAGS) src/jfen.c keep=$*

obj/os.a: src/os.c src/trivial.h
	$(COMPILE) $(CFLAGS) src/os.c keep=$*

obj/initdesk.a: src/initdesk.c src/initdesk.h
	$(COMPILE) $(CFLAGS) src/initdesk.c keep=$*

obj/sp.a: src/sp.c src/sp.h
	$(COMPILE) $(CFLAGS) src/sp.c keep=$*

obj/io.a: src/io.c src/io.h
	$(COMPILE) $(CFLAGS) src/io.c keep=$*

obj/ticons.a: src/ticons.asm src/fond.pak.asm
	$(ASSEMBLE) src/ticons.asm keep=$*

obj/sons.a: src/sons.asm src/clap.asm
	$(ASSEMBLE) src/sons.asm keep=$*

macros/dlgdata.mac: src/dlgdata.asm
	$(MACGEN) src/dlgdata.asm macros/dlgdata.mac 2/ainclude/m16.= 2/appleutil/m16.Util2

obj/dlgdata.a: src/dlgdata.asm macros/dlgdata.mac
	$(ASSEMBLE) src/dlgdata.asm keep=$*
