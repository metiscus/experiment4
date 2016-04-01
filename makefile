CC= gcc
CFLAGS= -g -Wall -Wextra
LDFLAGS= -lm

default:
	$(CC) $(CFLAGS) -c war.h war.c
	$(CC) $(CFLAGS) -c game.c
	$(CC) $(CFLAGS) -o game war.o game.c

clean:
	-rm -f game *.o *.gch
