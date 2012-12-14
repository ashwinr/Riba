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


$(OUTPUT): $(OBJECTS)
	rm -f $@
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(OUTPUT)
	
$(OUTPUT)-snappy: $(OBJECTS)
	rm -f $@
	$(CC) $(OBJECTS) $(LDFLAGS_SNAPPY) -o $(OUTPUT)
