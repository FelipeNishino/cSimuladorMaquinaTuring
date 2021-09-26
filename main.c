#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define TAPE_SIZE 100
#define ALPHABET_SIZE 30

typedef enum Direction {
	E,
	D
} Direction;

typedef struct Transition {
	int from;
	char read;
	char write;
	Direction dir;
	int target;
} Transition;

typedef struct TuringMachine {
	char inputAlphabet[ALPHABET_SIZE];
	int nStates;
	int nTransitions;
	Transition* transitions;
	char* tape[TAPE_SIZE];
	int headPosition;
	char** testWords;
	int wordCount;
} TuringMachine;

void loadMachine(const char* filename, TuringMachine* machine);
void runMachine(TuringMachine machine);
int parseDirection(char dir);

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Digite o nome do arquivo como argumento");
		exit(EXIT_FAILURE);
	}
	struct TuringMachine machine;
	loadMachine(argv[1], &machine);
	runMachine(machine);
	exit(EXIT_SUCCESS);
}

void loadMachine(const char* filename, TuringMachine* machine) {
	FILE *fp;
	char basename[20] = "./";
	char lineBuffer[255];
	for (int i = 0; i < TAPE_SIZE; ++i) {
		machine->tape[i] = "-";
	}

	strcat(basename, filename);
	fp = fopen(basename, "r");
	if (fp) {
		int lineCount = 0, targetLine = 100;
		int currentCharIndex = 0;
		char transitionBuffer[10] = "";
		int transitionReadStage = 0;
		int wordReadStage = 0;
		int nWords;
		int wordCounter = 0;
		int j;
		while (lineCount<targetLine) {
			fgets(lineBuffer, 255, fp);
			// printf("read:%s", lineBuffer);
			switch (lineCount) {
				case 0:
					memcpy(machine->inputAlphabet, lineBuffer, strlen(lineBuffer));
					break;
				case 1:
					machine->nStates = atoi(lineBuffer);
					break;
				case 2:
					machine->nTransitions = atoi(lineBuffer);
					machine->transitions = malloc(machine->nTransitions*sizeof(Transition));
					for (j = 0; j < machine->nTransitions; ++j) {
						fgets(lineBuffer, 255, fp);
						currentCharIndex = transitionReadStage = 0;

						while (lineBuffer[currentCharIndex] != '\0') {
							if (lineBuffer[currentCharIndex] != ' ' && lineBuffer[currentCharIndex] != 10) {
							    strncpy(transitionBuffer, lineBuffer + currentCharIndex, 1);
							}
							else {
								switch (transitionReadStage) {
									case 0:
										machine->transitions[j].from = atoi(transitionBuffer);
										break;
									case 1:
										machine->transitions[j].read = *transitionBuffer;
										break;
									case 2:
										machine->transitions[j].write = *transitionBuffer;
										break;
									case 3:
										machine->transitions[j].dir = parseDirection(*transitionBuffer);
										break;
									case 4:
										machine->transitions[j].target = atoi(transitionBuffer);
										break;
								}
								strcpy(transitionBuffer, "");
								transitionReadStage++;
							}
							currentCharIndex++;
						}
					}
					lineCount += machine->nTransitions;
					break;
				default:
					machine->wordCount = atoi(lineBuffer);
					machine->testWords = malloc(machine->wordCount);

					for (wordCounter; wordCounter < machine->wordCount; ++wordCounter) {
						fgets(lineBuffer, 255, fp);
						lineBuffer[strcspn(lineBuffer, "\n")] = '\0';
						machine->testWords[wordCounter] = malloc(strlen(lineBuffer) + 1);
						strcpy(machine->testWords[wordCounter], lineBuffer);
					}

					lineCount += machine->wordCount;
					targetLine = 4 + machine->nTransitions + machine->wordCount;
			}
    		// printf("line:%d, target:%d\n", lineCount, targetLine);
    		lineCount++;
		}
		printf("Done reading file.\n");
	}
	else {
		printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
	} 
	fclose(fp);
}

void runMachine(TuringMachine machine) {
	for (int i = 0; i < machine.nTransitions; ++i)
		printf("%d, %c, %c, %d, %d\n", machine.transitions[i].from, machine.transitions[i].read, machine.transitions[i].write, machine.transitions[i].dir, machine.transitions[i].target);
	for (int k = 0; k < machine.wordCount; ++k)
		printf("word %d: %s, size: %d\n", k, machine.testWords[k], strlen(machine.testWords[k]));
	for (int word = 0; word < count; ++word) {
		strncpy(machine.tape, machine.testWords[word]);
		if (strpbrk(machine.inputAlphabet, ) == NULL) {
			printf("symbol not in alphabet")
		}	
	}
	// printf("%d", strpbrk("asdf", "c") == NULL);
}

int parseDirection(char dir) {
	switch (dir) {
		case 'E':
			return E;
		case 'D':
			return D;
		default:
			return -1;
	}
	return -1;
}