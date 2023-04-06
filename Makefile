all:snakeNCurses

snakeNCurses: snakeNCurses.c
	gcc -Wall -o snakeNCurses snakeNCurses.c snakeNCurses.h -lncurses
	
clean:
	rm snakeNCurses

