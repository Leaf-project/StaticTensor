#include <stdio.h>

#include "../include/train.h"

/*
./bin/ptrses.exe sesfile datafile
*/

void ptrdata_(char *sesfile, char *datafile) {
	Session *ses = load_ses(sesfile);
	Data *data = load_data(ses, datafile);
	ptrdata(ses, data);
	freedata(ses, data);
	freeses(ses);
};

int main(int argc, char **argv) {
	if (argc == 2 && (!strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))) {
		printf("./bin/ptrses.exe sesfile datafile\n");
	} else if (argc == 3) {
		ptrdata_(argv[1], argv[2]);
	} else {
		printf("We need 3 arguments but got %i.\n", argc-1);
	}
	return 0;
}