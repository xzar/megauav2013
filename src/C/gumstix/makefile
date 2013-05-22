
DEBUG=yes
CC=gcc

ifeq ($(DEBUG),yes)
	CFLAGS=-W -Wall -g 
	LDFLAGS=-lcurses -lncurses -lm -lpthread
else
	CFLAGS=
	LDFLAGS=
endif

FOLDER=
EXEC=reseau
MAIN=main

SRC= $(wildcard $(FOLDER)*.c)
OBJ= $(SRC:$(FOLDER).c=$(FOLDER).o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(MAIN).o:

%$(FOLDER).o: %$(FOLDER).c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean: mrproper
	rm $(FOLDER)*.o

mrproper: 
	rm $(FOLDER)$(EXEC)

execute:
	./$(EXEC)

