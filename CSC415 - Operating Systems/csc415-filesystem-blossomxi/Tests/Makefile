# Make rules follow the format
# target : prerequisite
#	recipe

CC=gcc
CFLAGS=-Wall -I .. -c
PARENT=../
CFILES=test_parse_path.c $(PARENT)parsePath.c 
OBJECTS=test_parse_path.o $(PARENT)parsePath.o
BINARY=bin

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARY)

# Regular expression where % is a wild card.
%.o: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o $(OBJECTS)

clean : 
	rm $(BINARY) $(OBJECTS)

run : 
	./$(BINARY)