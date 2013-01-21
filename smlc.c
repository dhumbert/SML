#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMTABLESIZE 100
#define SMLMEMSIZE 100

typedef struct tableEntry {
    int symbol;
    char type; /* C, L, or V */
    int location;
} TableEntry;

TableEntry* symbolTable[SYMTABLESIZE];
char *smlMem[SMLMEMSIZE];
int instructionCounter = 0, symbolCounter = 0;
int smlMemCounter = SMLMEMSIZE - 1;

int getVariableLocation(char);
void saveCompiledSource(void);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("No input file specified.\n");
        return 0;
    }

    FILE *sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL) {
        printf("Invalid source file.\n");
        return 0;
    }

    while (!feof(sourceFile)) {
        char line[150];
        if (fgets(line, 150, sourceFile) != NULL) {
            char *tok;
            int tokCounter = 0;

            tok = strtok(line, " ");
            
            while (tok != NULL) {
                if (tokCounter == 0) { /* first token is line number */
                    TableEntry *entry = malloc(sizeof(TableEntry));
                    entry->symbol = atoi(tok);
                    entry->type = 'L';
                    entry->location = instructionCounter;
                } else if (tokCounter == 1) { /* second token is the command */
                    if (strcmp(tok, "input") == 0) {
                        /* get next token, which will be variable */
                        tok = strtok(NULL, " ");
                        char var = tok[0];
                        int location = getVariableLocation(var);
                        char instruction[150];
                        sprintf(instruction, "10%d", location);
                        smlMem[instructionCounter] = instruction;
                    } else if (strcmp(tok, "print") == 0) {
                        /* get next token, which will be variable */
                        tok = strtok(NULL, " ");
                        char var = tok[0];
                        int location = getVariableLocation(var);
                        char instruction[150];
                        sprintf(instruction, "11%d", location);
                        smlMem[instructionCounter] = instruction;
                    } else if (strcmp(tok, "rem") == 0) {
                        instructionCounter--; /* hack, rem = remark or comment. not in output */
                    }

                    instructionCounter++;
                }

                tok = strtok(NULL, " ");
                tokCounter++;
            }
        }
    }

    saveCompiledSource();
}

/* given a variable, find or add to the symbol table 
 * and return SML memory location */
int getVariableLocation(const char var) {
    int i, symbol = (int)var;
    for (i = 0; i < SYMTABLESIZE; i++) {
        if (symbolTable[i] != NULL) {
            if (symbolTable[i]->symbol == symbol && symbolTable[i]->type == 'V') {
                return symbolTable[i]->location;
            }
        }
    }

    /* if we got here, the symbol doesn't exist in the table. add it. */
    TableEntry *entry = malloc(sizeof(TableEntry));
    entry->symbol = symbol;
    entry->type = 'V';
    entry->location = smlMemCounter;

    symbolTable[symbolCounter++] = entry;
    return smlMemCounter--;
}

void saveCompiledSource() {
    FILE *outputFile = fopen("out.sml", "w");
    if (outputFile != NULL) {
        int i;
        for (i = 0; i < SMLMEMSIZE; i++) {
            if (smlMem[i] != NULL) {
                char *instruction = smlMem[i];
                fputs(instruction, outputFile);
                fputs("\n", outputFile);
            }
        }

        fputs("4300", outputFile);

        fclose(outputFile);
    }
}