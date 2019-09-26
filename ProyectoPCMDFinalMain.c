#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"ProyectoPCMDFinal.h"

static const char NEG = '~';

int main(){
	setbuf(stdout, NULL);
	int cadenaValida = 0;	//bandera
	char cadena [100];
	puts("Bienvenido, por favor escribe una fórmula lógica y te diré si es una fórmula bien formada.");
	while(cadenaValida==0)
	{
		gets(cadena);
		quitaEspacios(cadena);
		if(validacionCadena(cadena))
			cadenaValida = 1;
		else
			puts("Fórmula inválida, escribe otra por favor");
	}
	traduce(cadena);

	Stack s1 = create_stack();
	Stack s2 = create_stack();

	int isFbf = 1;	//bandera: es fórmula bien formada
	Node root = NULL;
	int cursor = 0;

	do{//MOOD PUSH: leer cadena e ir convirtiendo sus caracteres en nodos que se insertarán en stack1, hasta alcanzar un ')' o el fin de cadena

		for (int i = cursor; cadena[i]!='\0'; i++)
		{
			Node n = (Node) malloc(getsizeofstructnode());	//Tokenizar: ir convirtiendo la cadena en nodos
			setChar(n, cadena[i]);
			setLeft(n, NULL);
			setRight(n, NULL);

			stack_push(n, s1);	//Que se insertarán en la pila A
			if(cadena[i] == ')')
			{
				cursor = i + 1;
				break;
			}
		}

		//MODO POP: hará pop del stack1 y se procesará cada nodo según su tipo y según si está completo o no
		do{	//Controla modo pop
			Node N = stack_pop (s1);
			Node aux0, aux1, aux2;

			if (getChar(N) == ')' || (getChar(N) <= 'z' && getChar(N) >= 'a') || isComplete(N))	//Es un CIERRE DE PARÉNTESIS/PROP ATÓMICA
				stack_push(N, s2);	//Insertar nodo en la pila B.

			else if (getChar(N) == NEG)	//Es UNARIO
			{
				Node aux1 = stack_pop(s2);	//Extraer un nodo de la pila B, que deberá ser un nodo completo.
				if (!isComplete(aux1))
				{
					destroy(aux1);
					free(N);
					isFbf = 0;
				}
				else
				{
					setLeft(N, aux1);	//Y convertirlo en el único hijo de N.
					stack_push(N, s2);	//Ahora que N es un nodo completo, insertarlo en la pila B.
				}
			}

			else if (getChar(N) == '&' || getChar(N) == '|' || getChar(N) == '-' || getChar(N) == '<')	//Es BINARIO
			{
				aux1 = stack_pop(s1);	//Extraer un nodo de la pila A


				if (!isComplete(aux1) && (getChar(aux1) == '(' || getChar(aux1) == ')' || getChar(aux1) == NEG || getChar(aux1) == NEG ))
				{
					destroy(aux1);
					free(N);
					isFbf = 0;
				}
				else{
					setLeft(N, aux1);	//Y convertirlo en el hijo izquierdo de N
				aux2 = stack_pop(s2);	//Extraer un nodo de la pila B

				if (!isComplete(aux2) && (getChar(aux2) == '(' || getChar(aux2) == ')' || getChar(aux1) == NEG || getChar(aux1) == NEG))
				{
					destroy(aux2);
					free(N);
					isFbf = 0;
				}
				else{
					setRight(N, aux2);	//Y convertirlo en el hijo derecho de N.
				//N está completo y es la expresion binaria
					aux2 = stack_pop(s2);	//Sacar un nodo de la pila B
					if (getChar(aux2) != ')')
						isFbf = 0;

					aux1 = stack_pop(s1);	//Sacar otro nodo de la pila A
					if (getChar(aux1) != '(' && getChar(aux1) != NEG)
					{
						destroy(aux2);
						destroy(aux1);
						destroy(N);
						isFbf = 0;
					}
					else if (getChar(aux1) == NEG)	//Caso especial
					{
						setLeft (aux1, getLeft(N));
						setLeft(N, aux1);
						aux0 = stack_pop(s1);	//Sacar otro nodo de la pila B que sera el paréntesis abierto
						free(aux0);	//Elimina (apertura),
						free (aux2);	//Elimina (cierre)
						stack_push(N, s1);
					}
					else	//Caso normal
					{
						free(aux1);	//Eliminar apertura
						free (aux2);	//Eliminar cierre
						stack_push(N, s1);
					}
				}
				}

			}//Termina caso binario


			else	//es una APERTURA DE PARÉNTESIS
			{
				aux1 = stack_pop(s2);	//sacar un nodo de la pila B
				if (!isComplete(aux1))
				{
					destroy(aux1);
					free(N);
					isFbf = 0;
				}
				aux2 = stack_pop(s2);	//Sacar otro nodo de la pila B
				if (getChar(aux2) != ')')
					isFbf = 0;
				free(N);	//Eliminar (apertura),
				free (aux2);	//Eliminar (cierre)
				stack_push(aux1, s1);
			}

			if(stack_size(s1) == 0)	//Si en el modo POP la pila A se queda vacía
			{
				if (stack_size(s2)!= 1)
					isFbf = 0;	//Si no es sólo 1 nodo el que quedará en B, entonces la fórmula no es una fbf
				else
				{
					N = stack_pop(s2);	//La pila B debe tener un solo nodo (completo), que se extraerá de ahí.
					if (!isComplete(N))
					{
						destroy(N);
						isFbf = 0;
					}
					else
						root = N;	//Ya ambas pilas quedan vacías y ese último nodo de la pila B es la raíz del árbol
				}
			}
		}while (stack_size (s2) != 0 && isFbf);	//Cambiaremos al modo PUSH cuando la pila B vuelva a estar vacía.
	}while (stack_size (s1) != 0 && isFbf);	//Saldremos de ambos modos cuando queden vacías ambas pilas

	//---------------------------------------------------------------------------------------------------------------

	if (isFbf)
	{
		puts("Felicidades, es una es fórmula bien formada.");
		puts("Esta es su representación en notación polaca inversa:");
		print(root);
	}

	else puts("Lo siento, NO es una es fórmula bien formada.");

	stack_destroy(s1);
	stack_destroy(s2);
	destroy(root);	//Destruye el árbol
	return 0;
}

