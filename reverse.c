#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Estructura para almacenar cada línea
typedef struct Node {
    char *line;
    struct Node *next;
} Node;

// Función para imprimir el uso correcto
void print_usage() {
    fprintf(stderr, "usage: reverse <input> <output>\n");
}

int main(int argc, char *argv[]) {
    // Verificación de argumentos
    if (argc > 3) {
        print_usage();
        exit(1);
    }

    // Variables para archivos de entrada y salida
    FILE *input_file = NULL;
    FILE *output_file = NULL;

    // Apertura de archivos de entrada y salida
    if (argc >= 2) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        output_file = fopen(argv[2], "w");
        if (output_file == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    // Lectura del archivo de entrada y almacenamiento en una lista enlazada
    char *line = NULL;
    size_t line_length = 0;
    Node *head = NULL;

    while (getline(&line, &line_length, input_file) != -1) {
        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        new_node->line = strdup(line); // strdup() duplica la cadena
        new_node->next = head;
        head = new_node;
    }
    free(line);
    line = NULL;

    // Impresión de las líneas en orden inverso
    Node *current = head;
    while (current != NULL) {
        if (output_file != NULL) {
            fprintf(output_file, "%s", current->line);
        } else {
            printf("%s", current->line);
        }
        Node *temp = current;
        current = current->next;
        free(temp->line); // Liberar la memoria asignada a la línea
        free(temp); // Liberar la memoria asignada al nodo
    }

    // Cierre de archivos
    if (input_file != NULL) {
        fclose(input_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }

    return 0;
}
