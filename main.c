#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "turingmachine.h"

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
