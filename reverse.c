#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 1000

void tooManyArguments() {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
}

void openFile(char *file_name, FILE **file) {
    *file = fopen(file_name, "r");
    // Input file does not exist (Test 2) and Cannot open file (Test 3)
    if (*file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", file_name);
        exit(1);
    }
}

void hardlinkTest(char *argv[]) {
    struct stat stat_in, stat_out;

    // Obtener información del archivo de entrada
    if (stat(argv[1], &stat_in) == -1) {
        fprintf(stderr, "Error trying to get inputfile info'\n");
        exit(1);
    }

    // Obtener información del archivo de salida
    if (stat(argv[2], &stat_out) == -1) {
        fprintf(stderr, "Error trying to get outputfile info'\n");
        exit(1);
    }

    // Comparar números de dispositivo e inodo
    if (stat_in.st_dev == stat_out.st_dev && stat_in.st_ino == stat_out.st_ino) {
        fprintf(stderr, "reverse: input and output file must differ\n");        
        exit(1);
    }
}

void reverseLines(char lines[][MAX_LINE_LENGTH], int numLines) {
    // Print lines in reverse order
    for (int i = numLines - 1; i >= 0; i--) {
        printf("%s", lines[i]);
    }
}
 
void readLinesFromConsoleAndPrint() {
    char lines[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int numLines = 0;

    // Read lines from stdin
    while (fgets(lines[numLines], MAX_LINE_LENGTH, stdin) != NULL) {
        numLines++;
    }

    // Reverse and print lines
    reverseLines(lines, numLines);
}

void readLinesFromFileAndPrint(FILE **file) {

    char lines[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int numLines = 0;

    // Read lines from file
    while (fgets(lines[numLines], MAX_LINE_LENGTH, *file) != NULL) {
        numLines++;
    }

    // Reverse and print lines
    reverseLines(lines, numLines);

    fclose(*file);
}

void reverseFileAndWrite(FILE **inputFile, FILE **outputFile) {
    
    char lines[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int numLines = 0;

    // Read lines from input file
    while (fgets(lines[numLines], MAX_LINE_LENGTH, *inputFile) != NULL) {
        numLines++;
    }

    // Write lines to output file in reverse order
    for (int i = numLines - 1; i >= 0; i--) {
        fputs(lines[i], *outputFile);
    }

    fclose(*inputFile);
    fclose(*outputFile);

}

int main(int argc, char *argv[]) {
    FILE *input_file = NULL;
    FILE *output_file = NULL;

    switch (argc) {
        case 1:
            readLinesFromConsoleAndPrint();
            break;
        case 2:
            openFile(argv[1], &input_file);
            readLinesFromFileAndPrint(&input_file);
            break;
        case 3:
            // Input has the same name as output (Test 4)
            if (strcmp(argv[1], argv[2]) == 0) {
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            } else {
                openFile(argv[1], &input_file);
                openFile(argv[2], &output_file);

                // Input and output are actually the same file (hardlinked) (Test 5)
                hardlinkTest(argv);

                reverseFileAndWrite(&input_file, &output_file);
            }
            break;
        default:
            // Too many command-line arguments (Test 1)
            tooManyArguments();            
    }

    return 0;
}
