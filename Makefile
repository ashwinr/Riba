CC = g++

CFLAGS = -c -Wall -I.
LDFLAGS= -lleveldb -lreadline -lncurses -lpthread
OUTPUT = riba

OBJECTS = \
  lex.yy.o \
  riba.tab.o \
  riba.o \
  main.o

all: $(OUTPUT)

clean:
	-rm -f $(OUTPUT) *.o riba.tab.* lex.yy.*

riba.tab.c riba.tab.h: riba.y
	bison -d riba.y

lex.yy.c: riba.l riba.tab.h
	flex riba.l

$(OUTPUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
