// Librerias a incluir
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Prototipos de funciones

// Pantallas del juego

void imprimirtitulo();	//Imprime el título en la terminal.
void gameintro(WINDOW *win, int Y, int X );	//Función que genera la pantalla inicial del juego
void game(WINDOW *win, int Y, int X);		//Función que genera el juego en la terminal.
void gameoverscreen(WINDOW *win, int Y, int X);	//Función que genera la pantalla de game over del juego.

// Funciones para generar el juego

void marco(int Y, int X);	//Función que genera un marco de colores del tamaño de la terminal.
void marco_ingame(int Y, int X);	//Función que genera un 
int** snakes();		//Función que genera la posición inicial de la serpiente.
int** newsnake(int **snake);	//Función que incrementa en 1 el tamaño de la serpiente y extiende "la cola".


int* cookies(int Y, int X);	//Genera las coordenadas x, y de las "galletas" en el rango del mapa.
void printcookie(int *cookie, int Y, int X);	//Imprime la galleta generada en la función "cookies".
void comergalleta(int *cookie);		//Función que verifica si la serpiente ha "comido" la galleta.

void scoreingame();	//Función que almacena el puntaje obtenido.

// Funciones para geerar el movimiento

void input();	//Cambia el valor de la variable "bandera" para "dar dirección" a la serpiente.
int** movimiento(int **snake);	//De acuerdo al valor de bandera genera el "movimiento" del arreglo serpiente.
void imprimirsnake(int **snake);	//Función que imprime la "serpiente" en el "mapa".
