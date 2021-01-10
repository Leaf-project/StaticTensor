#include <stdio.h>

#include "../include/train.h"

/*
./bin/ptrses.exe sesfile
*/

void ptrses_(char* file) {
	Session *ses = load_ses(file);
	ptrses(ses);
	freeses(ses);
};

int main(int argc, char **argv) {
	if (argc == 2 && (!strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))) {
		printf("./bin/ptrses.exe sesfile\n");
	} else if (argc == 2) {
		ptrses_(argv[1]);
	} else {
		printf("We need 1 arguments but got %i.\n", argc-1);
	}
	return 0;
}