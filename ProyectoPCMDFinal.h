
#ifndef PROYECTOPCMDFINAL_H_
#define PROYECTOPCMDFINAL_H_

int validacionCadena (char * c);
void quitaEspacios(char *c);
void traduce (char * c);

typedef struct node * Node;
int getsizeofstructnode();
int isComplete(Node n);
void print(Node node);
void destroy(Node n);
void setLeft(Node parent, Node child);
void setRight(Node parent, Node child);
Node getLeft (Node n);
char getChar(Node n);
void setChar(Node n, char c);

typedef struct stack * Stack;
Stack create_stack();
int stack_size(Stack s);
void stack_push(Node node, Stack s);
Node stack_pop(Stack s);
void stack_destroy(Stack s);

#endif /* PROYECTOPCMDFINAL_H_ */

