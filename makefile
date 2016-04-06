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
	$(CC) $(CFLAGS) -o game $(OBJ) -lm

clean:
	-rm -f game $(OBJ) *.d *.gch

-include *.d
