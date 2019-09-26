#include"ProyectoPCMDFinal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char NEG = '~';

//----------------------------------------FUNCIONES CADENA--------------------------------
int validacionCadena (char * c){
	int l = strlen(c);
	int numParentesis= 0;

	for(int i = 0; c[i]!= '\0'; i++){
		if(c[i]== '(' || c[i]== ')')
			numParentesis++;
	}

	if(numParentesis%2 != 0)
		return 0;

	if (c[l-1] != ')' || c[0] != '(')
		return 0;

	for (int i = 0; c[i+1] != '\0'; i++)
		if (c[i] == '-' && c[i+1] != '>')
			return 0;

		else if (c[i] == '<' && c[i+1] != '-')
			return 0;

	for (int i = 0; c[i] != '\0'; i++)
		if (c[i] >= 'A' && c[i] <= 'Z') {
			return 0;
		}

	return 1;	//Si llego a este punto, es porque paso todos los check points
}

void quitaEspacios(char *c)
{
	int count = 0;	//Esto es el largo de la cadena sin espacios (cuenta cada caracter no-espacio que vas procesando)
	for (int i = 0; c[i]!='\0'; i++)	//Recorre toda la cadena
		if (c[i] != ' ')
			c[count++] = c[i]; //Si el caracter actual no es espacio, lo pone en index count y luego incrementra count (post-incremento)
	c[count] = '\0';	//Agrega fin de cadena
}

void traduce(char * c){
	int count = 0;	//Esto es el largo de la cadena traducida (cuenta cada caracter traducible que vas procesando)
	for (int i = 0; c[i]!='\0'; i++)//Recorre toda la cadena
	{
		if (c[i] == '-')
		{
			c[count++] = c[i]; //Si el caracter actual es -, lo pone en index count y luego incrementra count (post-incremento)
			i = i + 1;	//Se traga el >
		}
		else if (c[i] == '<')
		{
			c[count++] = c[i];	//Si el caracter actual es <, lo pone en index count y luego incrementra count (post-incremento)
			i = i + 2;	//Se traga el ->
		}
		else c[count++] = c[i];
	}
	c[count] = '\0';	//Agrega fin de cadena
}


//----------------------------------------FUNCIONES TREE--------------------------------------------

typedef struct node{
	char c;
	struct node * left, *right;
}* Node;

int getsizeofstructnode(){
	return (sizeof(struct node));
}

int isComplete(Node n){	//Un nodo es completo si se cumple cualquiera de las condiciones
	if (n->c <= 'z' && n->c >= 'a')
		return 1;	//Corresponde a una proposición atómica
	else if ((n->c == '&' || n->c == '|' || n->c == '<' || n->c =='-') && n->left != NULL && n->right != NULL)
		return 1; //Es binario y tiene ambos hijos
	else if ((n->c == NEG) && (n->left != NULL && n->right == NULL))
		return 1;	///Es unario y tiene un y sólo un hijo
	else
		return 0;
}

void print(Node node){	//RECURSIVA... es llamada con la raíz, imprime la expresión en forma polaca
	if (node != NULL)	//Si el nodo node es nulo, no se hace nada.
	{
		print(node->left);
		print(node->right);
		if (node->c == '<') printf("<->");	//el data de node
		else if (node->c == '-') printf("->");
		else printf("%c", node->c);
	}
}

void destroy(Node n){	//RECURSIVA... es llamada con la raíz
	if(n == NULL) return;
	if(n->left  != NULL) destroy(n->left);
	if(n->right != NULL) destroy(n->right);
	free(n);
}

void setLeft(Node parent, Node child){
	if (parent != NULL)
		parent->left = child;
}

void setRight(Node parent, Node child){
	if (parent != NULL)
		parent->right = child;
}

Node getLeft(Node n){
	return n->left;
}

char getChar(Node n){
	return n->c;
}

void setChar(Node n, char c){
	n->c = c;
}


//---------------------------------------FUNCIONES STACK---------------------------------

typedef struct stackNode{
	Node node;
	struct stackNode * prior;
}* StackNode;

typedef struct stack{
	StackNode top;
	int size;
}* Stack;

Stack create_stack(){
	Stack s = (Stack) malloc (sizeof(struct stack));
	s->size = 0;
	s->top = NULL;
	return s;
}

int stack_size(Stack s)
{
	if(s == NULL)
		return -1;
	return s->size;
}

void stack_push(Node node, Stack s){
	StackNode n = (StackNode) malloc(sizeof(struct stackNode));
	if(n == NULL)
		return;
	n->node = node;
	n->prior = s->top;
	s->top = n;
	s->size++;
}

Node stack_pop(Stack s)
{
	if(s->size == 0) return NULL;
	Node top = s->top->node;
	StackNode temp = s->top;
	s->top = s->top->prior;
	s->size--;
	free(temp);
	return top;
}

void stack_destroy(Stack s)
{
	StackNode aux1, aux2;
	aux1 = s->top;

	while(aux1 != NULL)
	{
		aux2 = aux1->prior;
		free(aux1);
		aux1 = aux2;
	}
	free(s);
}

