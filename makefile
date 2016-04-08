CC= gcc
CFLAGS= -g -Wall -Wextra -MD
LDFLAGS= -lm

SRC=\
	war.c\
	brigade.c\
	weapon.c\
	random.c\
	utility.c\
	game.c\
	territory.c\
	lexer.c\

OBJ=$(SRC:.c=.o)

default: $(OBJ)
	$(CC) $(CFLAGS) -o game $(OBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) -o parser lexer.o utility.o parse.c $(LDFLAGS)

clean:
	-rm -f game parser $(OBJ) *.d *.gch

-include *.d
