PROGRAM = a_fly
CC      = g++
CFLAGS  = -g -ansi -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm


$(PROGRAM): Projekat_iz_grafike.o DrawUtils.o Spider.o Fly.o SpiderWeb.o Mesh.o MouseUtils.o OBJParser.o Structs.h
	$(CC) $(LDFLAGS) -o $(PROGRAM) Projekat_iz_grafike.o DrawUtils.o Spider.o Fly.o SpiderWeb.o Mesh.o MouseUtils.o OBJParser.o Structs.h $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
