#include "snakeNCurses.h"

// Variables globales
short tamano = 3;		//Tamaño inicial de la serpiente
int gameover; //Estado de gameover, termina el juego.
int flag = 2;	//Bandera para saber la tecla presionada.
int *cookie;	//Apuntador a un arreglo que contiene las coordenadas de la galleta.
int X, Y; //Variables que contienen las dimensiones de la consola.
int score;	//Variable que acumula el puntaje obtenido.


int impCentrado(WINDOW *win, char *s, int y)	// Función que comprueba que una cadena quepa en la terminal y la imprime en el centro de la linea y.
{
   int x, l;
   
  //Este macro escribe en las coordenadas Y y X el tamaño de la consola.
   getmaxyx(win, Y, X);
   l = strlen(s);
   
   //Se valida que el mensaje quepa en la consola.
   if (y<0 || y >= Y || l > X)
      return -1;

   x = (X-l)/2;
   mvwprintw(win, y, x, s);	// Se imprime la cadena en el centro de la línea y seleccionada.
   wrefresh(win);
   return 0;
}

//Función main
int main()
{
	WINDOW *win; //Se crea una variable apuntador a la ventana.
	win = initscr();  //Inicializamos la ventana.
	raw();	//Se activa el paso directo de caracteres desde la terminal.
	keypad(stdscr, TRUE);	//Se activa la entrada para el teclado.
	noecho();	//Los caracterés presionados no son visibles.
	getmaxyx(win, Y, X); //Se obtienen las dimensiones de la consola.
	
	 if(has_colors() == FALSE)		// Se valida que la terminal acepte colores.
	{	endwin();
		printf("Tu terminal no soporta color :c\n");
		exit(1);	
	}
	start_color();	// Se inicia el color.
	// Inicializamos los pares de colores.
	init_pair(0, COLOR_BLACK, COLOR_WHITE);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_GREEN);
	init_pair(8, COLOR_WHITE, COLOR_WHITE);
	init_pair(9, COLOR_RED, COLOR_GREEN);
	
	curs_set(0);	//Se oculta el cursor.
	
	if(Y<24 || X<80)	//Se verifica que la consola tenga un tamaño mínimo de 24x80.
	{
		clear();
		printw("\n Su terminal es muy pequeña, se requiere un tamaño mínimo de 80x24.");
		printw("\n\n Por favor, incremente las dimensiones de la ventana.");
		getch();
	}
	else
		gameintro(win, Y, X);	//Se llama a la pantalla inicial/principal del juego.
		
	endwin();	// Se termina el modo NCurses.
}

void gameintro(WINDOW *win, int Y, int X)
{
	attron(A_BOLD);
	imprimirtitulo();	//Se imprime el título del juego en la terminal.
	impCentrado(win, "INSTRUCCIONES:", 12);
	impCentrado(win, "Pulse la tecla 'a' para girar a la izquierda.", 14);
	impCentrado(win, "Pulse la tecla 's' para girar a la derecha.", 15);
	impCentrado(win, "Presione la tecla x para terminar el juego", 17);
	attron(A_BLINK);
	impCentrado(win, "PRESIONE CUALQUIER TECLA PARA INICIAR EL JUEGO...", 19);
	attroff(A_BLINK);
	mvprintw(Y-3, X-20, "\n  El tamaño de su terminal es de %i X %i. \n\n", X, Y);		// Se imprime el tamaño de la terminal.
	attroff(A_BOLD);
	marco(Y, X);	//Se llama a una función que imprimir un marco en la terminal.
	getch();
	clear();
	game(win, Y, X);	//Se llama a la función que genera el juego
	refresh();
}

//Construcción del mapa y de los elementos
void game(WINDOW *win, int Y, int X)	//Esta 
{	
	//Se inicializa la 
	cookie = cookies(Y, X);		//Se genera la primera galleta
	printcookie(cookie, Y, X);	//Se imprime la galleta en el juego.
	int **snake = snakes();		//Se genera la primera ubicación en el mapa de la "serpiente" y se almacena en el arreglo "snake".
	imprimirsnake(snake);		//Se imprime la serpiente en el mapa de juego.
	
	nodelay(win, TRUE);		//Hace que la lectura no sea bloqueante en la ventana.
	while(!gameover)	
	{
		input();	//Se lee la entrada del jugador.
		flag = flag % 4;
		if(flag < 0)
			flag = flag + 4;
		clear();	//Se limpia la consola.
		marco_ingame(Y, X);		//Se crea el marco de juego, se imprime la barra superior.
		snake = movimiento(snake);	//Función que "genera" el movimiento de la serpiente.
		imprimirsnake(snake);
		printcookie(cookie, Y, X);	
		scoreingame();	//Se imprime el puntaje en la esquina superior derecha de la terminal.
		impCentrado(win, "Presione 'x' para salir", 0);
		refresh();
		if(snake[0][0] == cookie[0] && snake[0][1] == cookie[1])	//Se verifica si la serpiente "come la galleta".
			{
				comergalleta(cookie);
				snake = newsnake(snake);
			}	
	}
	nodelay(win, FALSE);	
	gameoverscreen(win, Y, X);	
}

void gameoverscreen(WINDOW *win, int Y, int X)
{
	clear();
	marco(Y, X);
	attron(A_BOLD);
	imprimirtitulo();
	
	mvprintw((Y/2)-1, (X/2)-9, "SU PUNTAJE ES: %i", score);
	impCentrado(win, "EL JUEGO HA TERMINADO", (Y/2)+1);
	attron(A_BLINK);
	impCentrado(win, "Presione cualquier tecla para salir del juego...", Y-8);
	attroff(A_BLINK);
	impCentrado(win, "REALIZO: JESUS ALBERTO MENDEZ VELA, JOSUE EMANUEL TORRES COLLI", Y-2);
	attroff(A_BOLD);
	getch();
	curs_set(0);
}

//Generador de galletas
int* cookies(int Y, int X)
{
    static int cookie[2];	//Se define un arreglo para las coordenadas de la galleta.
    cookie[0]= rand() % Y-1;	//Se genera las coordenadas de la galleta en los límites del mapa.
    cookie[1]= rand() % X-3;
    if(cookie[0] < 2)	//Se valida que las coordenadas de la galleta estén dentro del area y si no, se les pone el valor mínimo.
		cookie[0] = 2;	
	if(cookie[1] < 3)
		cookie[1] = 3;
    
    return cookie;	//Se regresa la dirección del arreglo que contiene las coordenadas de la galleta.
}

//init snake
int **snakes()
{

    int **snake;
    snake = malloc(sizeof(int*) * 3);	//Se reserva memoria para 3 apuntadores a arreglos de coordenadas.
    for(int i=0;i<3;i++)
    {
        snake[i]=malloc(sizeof(int*) * 2);	//Se reserva memoria para las coordenadas de cada elemento en la serpiente.
    }
	
	//Se definen las coordenadas inicial de cada "elemento" del arreglo "snake".
    snake[0][0]=15;
    snake[0][1]=35;
    snake[1][0]=15;
    snake[1][1]=34;
    snake[2][0]=15;
    snake[2][1]=33;
    return snake;	//Regresa el apuntador al arreglo generado.
}

void printcookie(int *cookie, int Y, int X)
{
	attron(COLOR_PAIR(8));
	mvprintw(cookie[0], cookie[1], "C");	//Se imprime la galleta con el simbolo C.
	attroff(COLOR_PAIR(8));
}

//Captura las teclas para mover la serpiente
void input()
{	
	switch (getch())
	{
		case 'a':
			flag += 1;
			break;
		case 's':
			flag -= 1;
			break;
		case 'x':
			gameover = 1;
			break;
		default:
			break;
	}
}

int** movimiento(int **snake)
{
	usleep(120000);
	int x;
	switch(flag)	//De acuerdo al valor de la variable bandera, se modifican los valores correspondientes para generar el "movimiento" de la "serpiente".
	{
		//A LA IZQUIERDA
		case 0:
			for(x = tamano-1; x>-1; x--)
			{
				if(x == 0)
				{
					snake[x][1]=snake[x][1]-1;
					if(snake[x][1]==1)	//Este es el código que detecta si la serpiente "sale" del mapa, ingrese por el lado "opuesto".
					{	
						snake[x][1]=X-3;	//En este caso, si la "serpiente sale" del lado izquierdo, se genera nuevamente en el lado derecho del mapa.
					}
				}
				else
				{
					snake[x][1]=snake[x-1][1];
					snake[x][0]=snake[x-1][0];
				}
				refresh();
			}
			break;
			
		//Hacia abajo	
		case 1:
			for(int x=tamano-1;x>-1;x--)
			{
				if(x==0)
				{
					snake[x][0]=snake[x][0]+1;
					if(snake[x][0]==Y-1)
					{
						snake[x][0]=2;	//En este caso, si la "serpiente sale" por la parte inferior del mapa, "aparece" nuevamente en la parte superior.
					}
				}
				else
				{
					snake[x][0]=snake[x-1][0];
					snake[x][1]=snake[x-1][1];
				}
			}
			break;
			
		//Hacia la derecha
		case 2:
			for(int x=tamano-1;x>-1;x--)
			{
				if(x==0)
				{
					snake[x][1]=snake[x][1]+1;
					if(snake[x][1]==X-2)
					{
						snake[x][1]=2;	//En este caso, si la "serpiente sale" del lado derecho, se genera nuevamente en el lado izquierdo del mapa.
					}
				}
				else
				{
					snake[x][1]=snake[x-1][1];
					snake[x][0]=snake[x-1][0];
				}
			}
			break;
		
		//Hacia arriba
		case 3:
			for(int x=tamano-1;x>-1;x--)
			{
				if(x==0)
				{
					snake[x][0]=snake[x][0]-1;
					if(snake[x][0]==1)
					{
						snake[x][0]=Y-2;	//En este caso, si la "serpiente sale" por la parte superior del mapa, "aparece" nuevamente en la parte inferior.
					}
				}
				else
				{
					snake[x][0]=snake[x-1][0];
					snake[x][1]=snake[x-1][1];
				}
			
			}
			break;
			
		default:
			break;
	}
	return snake;
}

void imprimirsnake(int **snake)
{
	int i;
	for(i=0; i<tamano; i++)
	{
		if(i == 0)
		{
			attron(COLOR_PAIR(7) | A_BOLD);
			mvprintw(snake[i][0], snake[i][1], "H");	//Se imprime la "cabeza" de la serpiente con el caracter "H".
			attroff(COLOR_PAIR(7) | A_BOLD);
		}
		else
		{
			attron(COLOR_PAIR(9));
			mvprintw(snake[i][0], snake[i][1], "/");	//Se imprime cada elemento del "cuerpo" de la serpiente con el caracter "/".
			attroff(COLOR_PAIR(9));
		}	
	}
}

void comergalleta(int *cookie)
{
	score += 1;	//Por cada vez que "se come la galleta" se incrementa el puntaje en 1.
	cookie = cookies(Y, X);	//Se generan las coordenadas para la siguiente galleta.
}

int** newsnake(int **snake)
{
	tamano += 1;	//Se aumenta en uno el tamaño de la serpiente.
	snake = realloc(snake, sizeof(int*) * tamano);	//Se relocaliza la memoria y se expande en 1 para generar una nueva "cola" de la "serpiente".
	snake[tamano-1] = malloc(sizeof(int*) * 2);	//Se reserva memoria para la nueva "cola" de la serpiente.
	snake[tamano-1][0] = snake[tamano-2][0];	//Se igualan las coordenadas a las de la "cola" anterior.
	snake[tamano-1][1] = snake[tamano-2][1];
	return snake;
}

void scoreingame()
{
	mvprintw(0, 1, "SNAKE in C - NCurses");	 //Se imprime el nombre de nuestro proyecto en la parte superior izquierda de la consola.
	mvprintw(0, X-12, "PUNTAJE: %i", score);	//Se imprime el puntaje hasta el momento en la parte superior derecha de la consola.
	mvprintw(0, X-18, "%i", flag);
}

void marco(int Y, int X)	// Esta función crea un marco de colores que se ajusta a la terminal. Su uso es meramente estetico.
{
	int i, color;
	Y -= 1;
	X -= 1;
	
	color = 0;		// Se asigna a color el valor 0.
	attron(A_BOLD);
	for(i=0; i<X; i++)
	{
		attron(COLOR_PAIR(color%6 + 1));	// De acuerdo al valor del modulo, se asigna un color diferente al carácter a imprimir.
		mvprintw(0, i, "*");	// Por cada iteración, el cursor se mueve "hacia la derecha" e imprime el caractér con el color correspondiente.
		attroff(COLOR_PAIR(i%6 + 1));
		color++;
	}
	
	for(i=0; i<Y; i++)
	{
		attron(COLOR_PAIR(color%6 + 1));
		mvprintw(i, X, "*");	// Por cada iteración, el cursor se mueve "hacia abajo" e imprime el caractér con el color correspondiente.
		attroff(COLOR_PAIR(i%6 + 1));
		color++;
	}
	for(i=X; i>0; i--)
	{
		attron(COLOR_PAIR(color%6 + 1));
		mvprintw(Y, i, "*");	// Por cada iteración, el cursor se mueve "hacia la izquierda" e imprime el caractér con el color correspondiente.
		attroff(COLOR_PAIR(i%6 + 1));
		color++;
	}
	for(i=Y; i>0; i--)
	{
		attron(COLOR_PAIR(color%6 + 1));
		mvprintw(i, 0, "*");	// Por cada iteración, el cursor se mueve "hacia arriba" e imprime el caractér con el color correspondiente.
		attroff(COLOR_PAIR(i%6 + 1));
		color++;
	}		
	attroff(A_BOLD);
}	// Termina la función generando un "marco" en "orilla" de la terminal.

void marco_ingame(int Y, int X)	// Esta función crea un marco que sirve como "borde" para el "mapa" de juego.
{
	int i;
	Y -= 1;
	X -= 1;
	
	attron(COLOR_PAIR(3) | A_BOLD);
	for(i=0; i<X+1; i++)
	{
		mvprintw(1, i, "-");	// Por cada iteración, el cursor se mueve "hacia la derecha" e imprime el caractér con el color correspondiente.
	}
	for(i=2; i<Y; i++)
	{
		mvprintw(i, X, "|");	// Por cada iteración, el cursor se mueve "hacia abajo" e imprime el caractér con el color correspondiente.
	}
	for(i=X; i>-1; i--)
	{
		mvprintw(Y, i, "-");	// Por cada iteración, el cursor se mueve "hacia la izquierda" e imprime el caractér con el color correspondiente.
	}
	for(i=Y-1; i>1; i--)  
	{
		mvprintw(i, 0, "|");	// Por cada iteración, el cursor se mueve "hacia arriba" e imprime el caractér con el color correspondiente.
	}
	attroff(COLOR_PAIR(3) | A_BOLD);
}	// Termina la función generando un "marco" en "orilla" de la terminal.

void imprimirtitulo()	//Función que escribe el título del juego en la terminal.
{
	int i;
	int y = 4, x = X/2;
	
	attron(COLOR_PAIR(8));
	//Escribe S
	for(i=-33; i<-23; i++)
		mvprintw(y, x+i, " ");
	for(i=-33; i<-31; i++)
		mvprintw(y+1, x+i, " ");
	for(i=-33; i<-23; i++)
		mvprintw(y+2, x+i, " ");
	for(i=-25; i<-23; i++)
		mvprintw(y+3, x+i, " ");
	for(i=-33; i<-23; i++)
		mvprintw(y+4, x+i, " ");
	
	//Escribe N
	for(i=0; i<5; i++)
		mvprintw(y+i, x-18, " ");
	for(i=0; i<5; i++)
		mvprintw(y+i, x-19, " ");
	mvprintw(y+1, x-17, " ");
	mvprintw(y+1, x-16, " ");
	mvprintw(y+2, x-15, " ");
	mvprintw(y+2, x-14, " ");
	mvprintw(y+3, x-13, " ");
	mvprintw(y+3, x-12, " ");
	for(i=0; i<5; i++)
		mvprintw(y+i, x-11, " ");
	for(i=0; i<5; i++)
		mvprintw(y+i, x-10, " ");
	
	//Escribe A
	mvprintw(y+2, x-3, " ");
	mvprintw(y+2, x-2, " ");
	mvprintw(y+1, x-3, " ");
	mvprintw(y+1, x-2, " ");
	mvprintw(y, x-1, " ");
	mvprintw(y, x, " ");
	mvprintw(y+2, x+1, " ");
	mvprintw(y+2, x+2, " ");
	mvprintw(y+1, x+1, " ");
	mvprintw(y+1, x+2, " ");
	for(i=-5; i<5; i++)
		mvprintw(y+3, x+i, " ");
	mvprintw(y+4, x-5, " ");
	mvprintw(y+4, x-4, " ");
	mvprintw(y+4, x+3, " ");
	mvprintw(y+4, x+4, " ");
	
	//Escribe K
	for(i=0; i<5; i++)
		mvprintw(y+i, x+8, " ");
	for(i=0; i<5; i++)
		mvprintw(y+i, x+9, " ");
	mvprintw(y+2, x+10, " ");
	mvprintw(y+2, x+11, " ");
	mvprintw(y+2, x+12, " ");
	mvprintw(y+1, x+12, " ");
	mvprintw(y+1, x+13, " ");
	mvprintw(y+1, x+14, " ");
	mvprintw(y+3, x+12, " ");
	mvprintw(y+3, x+13, " ");
	mvprintw(y+3, x+14, " ");
	mvprintw(y, x+14, " ");
	mvprintw(y, x+15, " ");
	mvprintw(y+4, x+14, " ");
	mvprintw(y+4, x+15, " ");
	mvprintw(y, x+16, " ");
	mvprintw(y, x+17, " ");
	mvprintw(y+4, x+16, " ");
	mvprintw(y+4, x+17, " ");
	
	//Escribe E
	for(i=0; i<5; i++)
		mvprintw(y+i, x+22, " ");
	for(i=0; i<5; i++)
		mvprintw(y+i, x+23, " ");
	for(i=23; i<31; i++)
		mvprintw(y, x+i, " ");
	for(i=23; i<29; i++)
		mvprintw(y+2, x+i, " ");
	for(i=23; i<31; i++)
		mvprintw(y+4, x+i, " ");
	
	attroff(COLOR_PAIR(8));	
}
