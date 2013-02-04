CC = g++

CFLAGS = -c -Wall -I.
LDFLAGS= -lleveldb -lreadline
LDFLAGS_SNAPPY=$(LDFLAGS) -lsnappy
OUTPUT = riba

OBJECTS = \
  lex.yy.o \
  riba.tab.o \
  riba.o \
  main.o

all: $(OUTPUT)
snappy: $(OUTPUT)-snappy

clean:
	-rm -f $(OUTPUT) *.o

riba.tab.c riba.tab.h: riba.y
	bison -d riba.y

lex.yy.c: riba.l riba.tab.h
	flex riba.l

$(OUTPUT): $(OBJECTS)
	rm -f $@
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OUTPUT)-snappy: $(OBJECTS)
	rm -f $@
	$(CC) $(OBJECTS) $(LDFLAGS_SNAPPY) -o $(OUTPUT)
