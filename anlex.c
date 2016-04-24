/*
 *	Analizador L�xico	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Pr�ctica de Programaci�n Nro. 1
 *	
 *	Descripcion:
 *	Implementa un analizador l�xico que reconoce n�meros, identificadores, 
 * 	palabras reservadas, operadores y signos de puntuaci�n para un lenguaje
 * 	con sintaxis tipo Pascal.
 *	
 */

/*********** Inclusi�n de cabecera **************/
#include "anlex.h"

/************* Variables globales **************/

int consumir;			/* 1 indica al analizador lexico que debe devolver
						el sgte componente lexico, 0 debe devolver el actual */

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente pascal
char buff[2*TAMBUFF];	// Buffer para lectura de archivo fuente
char id[TAMLEX];		// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/**************** Funciones **********************/


// Rutinas del analizador lexico

void error(const string mensaje)
{
	printf("\nLin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}

void sigLex()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
    
	while((c=fgetc(archivo))!=EOF)
	{
		if (c==' '){
            printf(" ");
	    	continue;	//eliminar espacios en blanco
        
        }        
        else if(c=='\t'){        
            printf("\t");
            continue;
        }
		else if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
            printf("\n");
			continue;
		}
		else if (isalpha(c))
		{
			//palabra reservada
			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
				if (i>=TAMLEX)
					error("Longitud de Identificador excede tama�o de buffer");
                    
			}while(isalpha(c));
			id[i]='\0';
			if (c != EOF){
				ungetc(c,archivo);}
			else
			    c=0;
			palabra_reservada(id);
            t.lexema = id;
			break;
		}
        else if (c=='"'){
            //cadena
            i=0;
            do{
                id[i]=c;
                i++;
                c=fgetc(archivo);
                if(i>=TAMLEX)
                    error("Longitud de Identificador excede tama�o de buffer");
            }while(c != '"' && i < TAMLEX);
            id[i] = c;
            id[i+1]='\0';
            /*if(c != EOF)
                ungetc(c,archivo);
            else
                c=0;*/
            t.compLex = LITERAL_CADENA;
            t.lexema = id;
            t.componente = nombres_comp[LITERAL_CADENA - 256];
            break;
        }

		else if (isdigit(c))
		{
                
				//es un numero
				i=0;
				estado=0;
				acepto=0;
				id[i]=c;
				while(!acepto)
				{
					switch(estado){
					case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					
					case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
                            printf("%c = ",c);
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;
    						id[++i]='\0';
    						acepto=1;
    						t.compLex=LITERAL_NUM;
                            t.lexema = id;
                            t.componente = nombres_comp[LITERAL_NUM - 256]; 
						break;
					case -1:
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else
                            sprintf(msg,"%c no esperado",c);
			                error(msg);
                            acepto=1;
                        break;
					}
				}
		}

		else if (c==':')
		{
			t.compLex= DOS_PUNTOS;
            t.lexema = ":";			
            t.componente = nombres_comp[DOS_PUNTOS - 256];
			break;
		}
		else if (c==',')
		{
			t.compLex= COMA;
            t.lexema = "," ;
            t.componente = nombres_comp[COMA - 256];            
			break;
		}		
        else if (c=='[')
		{
			t.compLex=L_CORCHETE;
			t.lexema = "[";
            t.componente = nombres_comp[L_CORCHETE - 256];
			break;
		}
		else if (c==']')
		{
			t.compLex=R_CORCHETE;
			t.lexema = "]";
            t.componente = nombres_comp[R_CORCHETE - 256];
			break;
		}
		else if (c=='{')
		{
            t.compLex= L_LLAVE;
            t.lexema = "{";            
            t.componente = nombres_comp[L_LLAVE - 256];
			break;		
        }
        else if (c=='}')
        {
            t.compLex= R_LLAVE;
            t.lexema = "}";            
            t.componente = nombres_comp[R_LLAVE - 256];
			break;		
        }
		else if (c!=EOF)
		{
			sprintf(msg,"%c no esperado",c);
            while(c != '\n'){
                c=fgetc(archivo);
            }
			error(msg);
		}
	}
	
    if (c==EOF)
	{
		t.compLex=EOF;
		// strcpy(e.lexema,"EOF");
		printf("EOF\n");
		exit(1);
	}
	
}

void palabra_reservada(char id []){
   if (strcmp(id,"TRUE") == 0 || strcmp(id,"true") == 0 ) {
       t.compLex = PR_TRUE;
       t.lexema = id;
       t.componente = nombres_comp[PR_TRUE - 256]; 
   }else if(strcmp(id,"FALSE") == 0 || strcmp(id,"false") == 0 ) {
       t.compLex = PR_FALSE;
       t.lexema = id;
       t.componente = nombres_comp[PR_FALSE - 256]; 
   }else if(strcmp(id,"NULL") == 0 || strcmp(id,"null") == 0 ) {
       t.compLex = PR_NULL;
       t.lexema = id;
       t.componente = nombres_comp[PR_NULL - 256]; 
   }
   else
       error("WTF");
}
int main(int argc,char* args[])
{
	// inicializar analizador lexico
	
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		while (t.compLex!=EOF){
			sigLex();
       		printf("%s ", t.componente);
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
