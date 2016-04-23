/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/

//Codigos

//#define L_CORCHETE      256
#define R_CORCHETE      257
#define L_LLAVE		    258
#define R_LLAVE		    259
#define COMA	        260
#define DOS_PUNTOS	    261
#define LITERAL_CADENA	262
#define LITERAL_NUM     256
#define PR_TRUE         264
#define PR_FALSE        265
#define PR_NULL         266
// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50

//const char * nombres_comp [] = {"L_CORCHETE"};


/************* Estructuras ********************/
typedef char * string;
typedef struct {
	int compLex;
	string lexema;
    string comp_nombre;
} token;

/************* Prototipos ********************/
void sigLex();
