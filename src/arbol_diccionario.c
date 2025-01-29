#include "arbol_diccionario.h"
#include <stdlib.h>
#include <string.h>

// Funcion para crear un nodo del BST
Node *createNode(const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->word, word);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Funcion para insertar en el BST
Node *insert(Node *root, const char *word) {
    if (!root) {
        return createNode(word); // Si es el primer nodo en el arbol, se coloca la palabra en el y se coloca como nodo root
    }

    if (strcmp(word, root->word) < 0) { // strcmp hace la comparacion de cadenas y devuelve un numero negativo si word es menor que root->word
        root->left = insert(root->left, word);
    } else if (strcmp(word, root->word) > 0) { // strcmp hace la comparacion de cadenas y devuelve un numero positivo si word es mayor que root->word
        root->right = insert(root->right, word);
    }

    return root;
}

// Funcion para recorrer el arbol en orden                                nodo actual
// Primero el contenido del nodo a la izquierda                           /        \
// Luego el contenido del nodo actual                                nodo izq     nodo der
// Luego el contenido del nodo a la derecha                           /    \      /     \
//                                                                  null  null  null    null
void writeInOrder(Node *root, FILE *file) {
    if (root) {
        writeInOrder(root->left, file); // Primero se recorre la rama izquierda, para llegar al dato mas pequenno segun el criterio de orden
        fprintf(file, "%s\n", root->word); // Luego se desarrolla la operacion que se desea realizar. Escribir en el archivo el dato del nodo actual.
        writeInOrder(root->right, file); // Luego se recorre la rama derecha, para completar la terna.
    }
}

// Funcion para buscar una palabra en el BST
bool search(Node *root, const char *word) {
    if (!root) {
        return false;
    }
    if (strcmp(word, root->word) == 0) { // Compara la palabra que se busca con la palabra en el nodo actual
        return true;
    }
    if (strcmp(word, root->word) < 0) { // Si la palabra es menor sintacticamente hablando la busqueda se mueve a la izquierda
        return search(root->left, word);
    }
    return search(root->right, word); // Si la palabra no igual o menor la busqueda se mueve a la derecha
}

// Liberar memoria del arbol nodo por nodo hasta llegar al root del arbol
void freeTree(Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

//int main() {
//    FILE *inputFile = fopen(INPUT_FILE, "r"); // Se abre el archivo con las palabras en desorden
//    if (!inputFile) {
//        perror("Error al abrir el archivo de entrada");
//        return EXIT_FAILURE;
//    }
//
//    Node *root = NULL;
//    char word[MAX_WORD_LENGTH];
//
//    // Leer palabras y llenar el arbol BST
//    while (fscanf(inputFile, "%s", word) != EOF) { // Se lee una linea del archivo con una palabra
//        root = insert(root, word); // Se inserta la palabra en el BST
//    }
//
//    fclose(inputFile);
//
//    // Crear archivo de salida con palabras ordenadas
//    FILE *outputFile = fopen(OUTPUT_FILE, "w");
//    if (!outputFile) {
//        perror("Error al crear el archivo de salida");
//        freeTree(root); // Si hay problemas para crear el archivo se libera toda la memoria del BST
//        return EXIT_FAILURE;
//    }
//
//    // Escribir palabras ordenadas en el archivo
//    writeInOrder(root, outputFile); // Recorre el arbol en orden y escribe las palabras en el archivo de salida
//    fclose(outputFile);
//
//    printf("Diccionario ordenado generado en '%s'.\n", OUTPUT_FILE);
//
//    // Busqueda interactiva en el arbol
//    char searchWord[MAX_WORD_LENGTH];
//    printf("Ingrese palabras para buscar en el diccionario (escriba 'salir' para terminar):\n");
//    while (true) {
//        printf("Palabra: ");
//        scanf("%s", searchWord);
//        if (strcmp(searchWord, "salir") == 0) {
//            break;
//        }
//        if (search(root, searchWord)) {
//            printf("La palabra '%s' SI esta en el diccionario.\n", searchWord);
//        } else {
//            printf("La palabra '%s' NO esta en el diccionario.\n", searchWord);
//        }
//    }
//
//    // Liberar memoria
//    freeTree(root);
//    return 0;
//}
