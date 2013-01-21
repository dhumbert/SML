#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMSIZE 100
#define READ 10
#define WRITE 11
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

void displayGreeting();
void executeProgram(int[]);

int main(int argc, char *argv[]) {
    int memory[MEMSIZE] = {0}, currentLocation = 0, currentWord = 0;

    /* if no filename passed, run interactive prompt */
    if (argc <= 1) {
        displayGreeting();

        while (1) {
            printf("%02d ? ", currentLocation);
            scanf("%d", &currentWord);
            getchar();

            if (currentWord == -99999) {
                printf("*** Program loading completed ***\n");
                printf("*** Program execution begins ***\n");
                executeProgram(memory);
                return 0;
            } else {
                memory[currentLocation] = currentWord;
            }

            currentLocation++;
        }
    } else {
        FILE *sourceFile = fopen(argv[1], "r");
        if (sourceFile == NULL) {
            printf("Invalid source file\n");
            return 0;
        } else {
            char line[150];

            while (!feof(sourceFile)) {
                if ( fgets(line, 150, sourceFile) != NULL) {
                    if (currentWord = atoi(line)) {
                        memory[currentLocation] = currentWord;
                    } else{
                        printf("Invalid command: %s\n", line);
                        exit(0);
                    }
                }
                currentLocation++;
            }

            executeProgram(memory);
            return 0;
        }
    }
}

void displayGreeting() {
    printf("\n");
    printf("***         Welcome to Simpletron!             ***\n");
    printf("*** Please enter your program one instruction  ***\n");
    printf("*** (or data word) at a time. I will type the  ***\n");
    printf("*** location number and a question mark (?).   ***\n");
    printf("*** You then type the word for that location.  ***\n");
    printf("*** Type -99999 to stop entering your program. ***\n");
    printf("\n");
}

void executeProgram(int memory[]) {
    int accumulator = 0, instructionCounter = 0, 
        instructionRegister = 0, operationCode = 0, operand = 0;

    while (instructionCounter < MEMSIZE) {
        instructionRegister = memory[instructionCounter];

        if (instructionRegister == 0) {
            continue;
        }

        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        #ifdef DEBUG
        printf("Opcode: %d, operand: %d\n", operationCode, operand);
        #endif
        
        switch (operationCode) {
            case READ:
                printf("SML Prompt > ");
                scanf("%d", &memory[operand]);
                break;
            case WRITE:
                printf("%d\n", memory[operand]);
                break;
            case LOAD:
                accumulator = memory[operand];
                break;
            case STORE:
                memory[operand] = accumulator;
                break;
            case ADD:
                accumulator += memory[operand];
                break;
            case SUBTRACT:
                accumulator -= memory[operand];
                break;
            case DIVIDE:
                accumulator /= memory[operand];
                break;
            case MULTIPLY:
                accumulator *= memory[operand];
                break;
            case BRANCH:
                instructionCounter = operand;
                continue;
                break;
            case BRANCHZERO:
                if (accumulator == 0) {
                    instructionCounter = operand;
                    continue;
                }
                break;
            case BRANCHNEG:
                if (accumulator < 0) {
                    instructionCounter = operand;
                    continue;
                }
            case HALT:
                exit(0);
        }
        instructionCounter++;
    }
}