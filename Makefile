TARGETS = trivial append control corrige merge

IIX = iix
CHTYP = iix chtyp
COMPILE = iix compile
ASSEMBLE = iix assemble
LINK = iix link
MACGEN = iix macgen

CFLAGS = -I -P cc=-i"src"

OBJECTS = obj/startup.a obj/main.a obj/dialogs.a obj/desk.a \
	obj/jfen.a obj/os.a obj/initdesk.a obj/sounds.a obj/sp.a \
	obj/io.a obj/ticons.a obj/sons.a obj/dlgdata.a

all : obj macros $(TARGETS)

.PHONY: clean
clean:
	rm $(TARGETS) obj/* macros/*
	rmdir obj macros

obj macros:
	mkdir $@

trivial: $(OBJECTS)
	$(LINK) \
	obj/startup obj/main obj/dialogs obj/desk obj/jfen obj/os \
	obj/initdesk obj/ticons obj/sons obj/dlgdata obj/sounds \
	obj/sp obj/io \
	keep=$@

append: obj/append.a
	$(LINK) $< keep=$@
	
control: obj/control.a
	$(LINK) $< keep=$@
	
corrige: obj/corrige.a
	$(LINK) $< keep=$@

merge: obj/merge.a
	$(LINK) $< keep=$@

obj/%.ROOT: obj/%.A

macros/startup.mac: src/startup.asm
	$(MACGEN) $< $@ 2/ainclude/m16.= 2/appleutil/m16.Util2

obj/startup.a: src/startup.asm macros/startup.mac src/trivial.pak.asm
	$(ASSEMBLE) $< keep=obj/$$

obj/main.a: src/main.c
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/dialogs.a: src/dialogs.c src/dialogs.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/desk.a: src/desk.c src/trivial.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/jfen.a: src/jfen.c src/trivial.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/os.a: src/os.c src/trivial.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/initdesk.a: src/initdesk.c src/initdesk.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/sounds.a: src/sounds.c src/sounds.h src/trivial.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/sp.a: src/sp.c src/sp.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/io.a: src/io.c src/io.h
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/ticons.a: src/ticons.asm src/fond.pak.asm
	$(ASSEMBLE) $< keep=obj/$$

obj/sons.a: src/sons.asm src/clap.asm
	$(ASSEMBLE) $< keep=obj/$$

macros/dlgdata.mac: src/dlgdata.asm
	$(MACGEN) $< $@ 2/ainclude/m16.= 2/appleutil/m16.Util2

obj/dlgdata.a: src/dlgdata.asm macros/dlgdata.mac
	$(ASSEMBLE) $< keep=obj/$$

obj/append.a: tools/append.c
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/control.a: tools/control.c
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/corrige.a: tools/corrige.c
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

obj/merge.a: tools/merge.c
	$(COMPILE) $(CFLAGS) $< keep=obj/$$

install: trivial
	cp base.po trivial.po
	ac.sh -p trivial.po Trivial s16 <$?
	scp trivial.po rpi4:/tnfs
