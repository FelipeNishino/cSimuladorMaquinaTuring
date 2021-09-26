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
} TuringMachine;

void loadMachine(const char* filename, TuringMachine* machine, char** testWords);
int parseDirection(char dir);

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Digite o nome do arquivo como argumento");
		exit(EXIT_FAILURE);
	}
	struct TuringMachine machine;
	char** testWords;
	loadMachine(argv[1], &machine, testWords);
	// for (int k = 0; k < 4; ++k) {
	// 	printf("word %d: %s, size: %d", k, testWords[k], strlen(testWords[k]));
	// }
	exit(EXIT_SUCCESS);
}

void loadMachine(const char* filename, TuringMachine* machine, char** testWords) {
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
		// char *transitionBuffer = malloc(10*sizeof(char));
		char transitionBuffer[10] = "";
		int transitionReadStage = 0;
		int wordReadStage = 0;
		int nWords;
		int wordCounter = 0;
		int j;
		while (lineCount<targetLine) {
			fgets(lineBuffer, 255, fp);
			printf("read:%s", lineBuffer);
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
						// transitionBuffer = "";

						while (lineBuffer[currentCharIndex] != '\0') {
							if (lineBuffer[currentCharIndex] != ' ') {
								// printf("ta aqui, tb: %s, lb:%s", transitionBuffer, lineBuffer);
								strncat(transitionBuffer, lineBuffer, 1000);
								// printf("ta aqui\n");
							}
							else {
								switch (transitionReadStage) {
									case 0:
										// printf("ta aqui 0\n");
										machine->transitions[j].from = atoi(transitionBuffer);
										break;
									case 1:
										// printf("ta aqui 1\n");
										machine->transitions[j].read = *transitionBuffer;
										break;
									case 2:
										// printf("ta aqui 2\n");
										machine->transitions[j].write = *transitionBuffer;
										break;
									case 3:
										// printf("ta aqui 3\n");
										machine->transitions[j].dir = parseDirection(*transitionBuffer);
										break;
									case 4:
										// printf("ta aqui 4\n");
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
					nWords = atoi(lineBuffer);
					testWords = malloc(nWords);

					for (wordCounter; wordCounter < nWords; ++wordCounter) {
						fgets(lineBuffer, 255, fp);
						testWords[wordCounter] = malloc(strlen(lineBuffer - 1));
						memcpy(testWords[wordCounter], lineBuffer, strlen(lineBuffer - 1));
						if (testWords[wordCounter][strlen(testWords[wordCounter]) - 1] == 10) {
							testWords[wordCounter][strlen(testWords[wordCounter]) - 1] = 0;
						}
						// for (int n = 0; n < strlen(testWords[wordCounter]); ++n) {
						// 	printf("%d#", testWords[wordCounter][n]);
						// }
						// printf("\n");
					}
					lineCount += nWords;
					targetLine = 4 + machine->nTransitions + nWords;
					for (int k = 0; k < 4; ++k) {
						printf("word %d: %s, size: %d\n", k, testWords[k], strlen(testWords[k]));
					}
			}
    		printf("line:%d, target:%d\n", lineCount, targetLine);
    		lineCount++;
		}
		printf("Done reading file.\n");
	}
	else {
		printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
	} 
	// printf("%d", strpbrk("asdf", "c") == NULL);
	fclose(fp);
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