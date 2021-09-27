#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#define TAPE_SIZE 100
#define ALPHABET_SIZE 30

typedef enum Direction {
	E = -1,
	D = 1
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
	char tape[TAPE_SIZE];
	int headPosition;
	char** testWords;
	int wordCount;
	int currentState;
} TuringMachine;

void loadMachine(const char* filename, TuringMachine* machine);
void runMachine(TuringMachine machine);
bool testWord(TuringMachine machine, char* word);
void printTape(char* tape, int headPos);
void resetTape(TuringMachine* machine);
int parseDirection(char dir);

int main(int argc, const char* argv[]) {
	// Pede o nome do arquivo como argumento da linha de comando
	if (argc < 2) {
		printf("Digite o nome do arquivo como argumento");
		exit(EXIT_FAILURE);
	}
	// Inicializa a máquina com o arquivo passado como argumento
	struct TuringMachine machine;
	loadMachine(argv[1], &machine);
	// Roda a simulação das palavras de teste
	runMachine(machine);
	// Finaliza o programa
	exit(EXIT_SUCCESS);
}

void loadMachine(const char* filename, TuringMachine* machine) {
	FILE *fp;
	char basename[20] = "./";
	char lineBuffer[255];
	int currentLine = 0, targetLine = 100;
	char transitionBuffer[10] = "";
	int transitionCharIndex = 0;
	int transitionInitStage = 0;
	int transitionInitIndex = 0;
	int wordCounter = 0;
	resetTape(machine);

	// Abre o arquivo com o caminho ".\filename"
	strcat(basename, filename);
	fp = fopen(basename, "r");
	// Checa se o fopen foi bem sucedido
	if (!fp)  {
		printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
		fclose(fp);
		return;
	}
	
	// O loop principal se baseis num valor arbritario, que sera calculado durante a leitura do arquivo
	while (currentLine<targetLine) {
		// Lê uma linha do arquivo
		fgets(lineBuffer, 255, fp);

		// Dependendo da linha atual, realiza um statement específico
		switch (currentLine) {
			// Leitura do alfabeto de entrada
			case 0:
				memcpy(machine->inputAlphabet, lineBuffer, strlen(lineBuffer));
				strcat(machine->inputAlphabet, "-");
				break;
			// Leitura do numero de estados
			case 1:
				machine->nStates = atoi(lineBuffer);
				break;
			// Leitura do numero de transições e das transições em si
			case 2:
				machine->nTransitions = atoi(lineBuffer);
				// Alocação de memória para as transições
				machine->transitions = malloc(machine->nTransitions*sizeof(Transition));
				// Inicializa (nTransisions) transições
				for (transitionInitIndex; transitionInitIndex < machine->nTransitions; ++transitionInitIndex) {
					// Lê a próxima linha 
					fgets(lineBuffer, 255, fp);
					// Reinicializa as variáveis de controle da inicialização
					transitionCharIndex = transitionInitStage = 0;
					// Enquanto não chegar no final da string
					while (lineBuffer[transitionCharIndex] != '\0') {
						// Concatena o valor lido no buffer de transição
						if (lineBuffer[transitionCharIndex] != ' ' && lineBuffer[transitionCharIndex] != 10) {
						    strncpy(transitionBuffer, lineBuffer + transitionCharIndex, 1);
						}
						else {
							// Quando lê um separador (\s\n) executa uma parte da inicialização
							switch (transitionInitStage) {
								case 0:
									machine->transitions[transitionInitIndex].from = atoi(transitionBuffer);
									break;
								case 1:
									machine->transitions[transitionInitIndex].read = *transitionBuffer;
									break;
								case 2:
									machine->transitions[transitionInitIndex].write = *transitionBuffer;
									break;
								case 3:
									machine->transitions[transitionInitIndex].dir = parseDirection(*transitionBuffer);
									break;
								case 4:
									machine->transitions[transitionInitIndex].target = atoi(transitionBuffer);
									break;
							}
							// Reseta o buffer de transição
							strcpy(transitionBuffer, "");
							// Avança o estado de inicialização
							transitionInitStage++;
						}
						transitionCharIndex++;
					}
				}
				// Compensa pelas iterações executadas no for interno
				currentLine += machine->nTransitions;
				break;
			// Inicializa as palavras de entrada
			default:
				machine->wordCount = atoi(lineBuffer);
				// Aloca o vetor principal com o numero de palavras
				machine->testWords = malloc(machine->wordCount);

				for (wordCounter; wordCounter < machine->wordCount; ++wordCounter) {
					// Lê a próxima palavra
					fgets(lineBuffer, 255, fp);
					// Remove o \n das palavras
					lineBuffer[strcspn(lineBuffer, "\n")] = '\0';
					// Aloca e inicializa cada palavra
					machine->testWords[wordCounter] = malloc(strlen(lineBuffer) + 1);
					strcpy(machine->testWords[wordCounter], lineBuffer);
				}
				// Compensa pelas iterações executadas no for interno
				currentLine += machine->wordCount;
				//  Define a targetLine para a condição de parada do while
				targetLine = 4 + machine->nTransitions + machine->wordCount;
		}
		currentLine++;
	}
	printf("Done reading file.\n");
	fclose(fp);
}

void runMachine(TuringMachine machine) {
	// Para cada palavra, executa os testes
	for (int wordIndex = 0; wordIndex < machine.wordCount; ++wordIndex) {
		// Reinicializa a máquina
		machine.headPosition = 0;
		machine.currentState = 1;
		resetTape(&machine);

		// Realiza o teste
		if (testWord(machine, machine.testWords[wordIndex])) printf("%d: %s OK\n", wordIndex + 1, machine.testWords[wordIndex]);
		else printf("%d: %s not OK\n", wordIndex + 1, machine.testWords[wordIndex]);
	}
}

bool testWord(TuringMachine machine, char* word) {
	// Escreve a palavra na fita
	strncpy(machine.tape, word, strlen(word));
	// Usa o estado de aceitação como condição de parada
	while (machine.currentState != machine.nStates) {
		// Lê o símbolo da fita na posição do cabeçote
		char* symbol = malloc(2 * sizeof(char));
		symbol[0] = machine.tape[machine.headPosition];
		symbol[1] = 0;

		// printTape(machine.tape, machine.headPosition);
		// printf("state: %d, symbol: %s\n", machine.currentState, symbol);

		// Checa se o símbolo lido pertence ao alfabeto
		if (strpbrk(machine.inputAlphabet, symbol) == NULL){
			// Caso não pertença, retorna um erro
			printf("Error: Symbol %c at pos %d not in alphabet", machine.tape[machine.headPosition], machine.headPosition);
			return false;
		}
		// Percorre as transições procurando alguma válida
		for (int transitionIndex = 0; transitionIndex < machine.nTransitions; ++transitionIndex) {
			// Tenta bater o estado atual e o símbolo lido com a transição
			if (machine.transitions[transitionIndex].from == machine.currentState && machine.transitions[transitionIndex].read == symbol[0]) {
				// Caso encontre, escreve o novo símbolo na fita
				machine.tape[machine.headPosition] = machine.transitions[transitionIndex].write;
				// Move o cabecote
				machine.headPosition += machine.transitions[transitionIndex].dir;
				// Verifica se o cabecote se encontra numa posição válida
				if (machine.headPosition < 0 || machine.headPosition > TAPE_SIZE){
					printf("Error: Head position exceeded tape boundaries.\n");
					return false;
				}
				// Seta o novo estado
				machine.currentState = machine.transitions[transitionIndex].target;
				// Verifica se a máquina está num estado válido
				if (machine.currentState < 1 || machine.currentState > machine.nStates) {
					printf("Error: Reached invalid state.\n");
					return false;
				}
				break;
			}
			// Caso a simulação não encontre uma transição válida, retorna um erro
			if (transitionIndex == machine.nTransitions - 1) {
				return false;
			}
		}
	}
	// Caso chegue ao estado de aceitação, retorna true e aceita a palavra
	return true;
}

void printTape(char* tape, int headPos) {
	// Printa toda a fita, com o cabeçote indicado por "*" ANTES do símbolo em que se encontra
	for (int i = 0; i < TAPE_SIZE; ++i) {
		if (i == headPos) {
			printf("*");
		}
		printf("%c", tape[i]);
	}
	printf("\n");
}

void resetTape(TuringMachine* machine) {
	// Marca todas as posições da fita com "-"
	for (int i = 0; i < TAPE_SIZE; ++i) machine->tape[i] = '-';
}

int parseDirection(char dir) {
	// Retorna o valor atribuído para cada direção
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