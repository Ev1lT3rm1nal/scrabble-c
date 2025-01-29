#include <stdbool.h>
#include <stdio.h>

#ifndef ARBOL_DICCIONARIO_H
#define ARBOL_DICCIONARIO_H


#define MAX_WORD_LENGTH 100

// Nodo del BST (Binary Search Tree)
typedef struct Node {
    char word[MAX_WORD_LENGTH];
    struct Node *left; // Hijo izquierdo donde van a a parar los nodos cuyo dato es menor al nodo padre
    struct Node *right; // Hijo derecho donde van a a parar los nodos cuyo dato es menor al nodo padre
} Node;


Node *createNode(const char *word);

Node *insert(Node *root, const char *word);

void writeInOrder(Node *root, FILE *file);

bool search(Node *root, const char *word);

void freeTree(Node *root);

#endif //ARBOL_DICCIONARIO_H
