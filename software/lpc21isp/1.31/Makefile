PROJECT=lpc21isp
SOURCES=options.c $(PROJECT).c
INCLUDES=options.h 
OPT = -lm
PUB = pub

#  $@ is the name of the file to be made.
# $? is the names of the changed dependents. 

all: $(PROJECT)

$(PROJECT):  $(SOURCES) $(INCLUDES)
	$(CC) $(OPT) $(SOURCES) -o $@

echo: $(SOURCES) $(INCLUDES)
	echo $?

dist:	all
	tar cvfz $(PROJECT).tgz $(SOURCES) $(INCLUDES)  Makefile 

web:	dist
	mkdir  /Volumes/$(PUB)/code/c/$(PROJECT)
	cp $(PROJECT).tgz /Volumes/$(PUB)/code/c/$(PROJECT)/

back:	dist
	mkdir  /Volumes/$(PUB)/all/code/c/$(PROJECT)
	cp *.c /Volumes/$(PUB)/all/code/c/$(PROJECT)/
	cp *.h /Volumes/$(PUB)/all/code/c/$(PROJECT)/

