CC = g++

CFLAGS = -c -Wall -I.
LDFLAGS= -lleveldb -lreadline
OUTPUT = riba

OBJECTS = \
  lex.yy.o \
  riba.tab.o \
  riba.o \
  main.o

all: $(OUTPUT)

clean:
	-rm -f $(OUTPUT) *.o

$(OUTPUT): $(OBJECTS)
	rm -f $@
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
