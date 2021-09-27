#ifndef TURINGMACHINE_H_INCLUDED
#define TURINGMACHINE_H_INCLUDED

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

#endif /* TURINGMACHINE_H_INCLUDED */
