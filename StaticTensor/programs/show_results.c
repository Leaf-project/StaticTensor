#include <stdio.h>

#include "../include/train.h"

/*
./bin/show_results.exe sesfile datafile loss score -v_by_v
*/

void show_results_(char *sesfile, char *datafile, char *strloss, char *strscorefn, char v_by_v) {
	Loss loss;
	if (!strcmp(strloss,"default"))		loss = mbe;
	else if (!strcmp(strloss,"mbe"))	loss = mbe;
	else if (!strcmp(strloss,"mse"))	loss = mse;
	else if (!strcmp(strloss,"mae"))	loss = mae;
	else if (!strcmp(strloss,"cel"))	loss = cel;
	else {
		printf("Not recognised loss %s\n", strloss);
		exit(1);
	}
	//Score
	Scorefn scorefn;
	if (!strcmp(strscorefn,"default"))	scorefn = sum_score;
	else if (!strcmp(strscorefn,"sum"))	scorefn = sum_score;
	else if (!strcmp(strscorefn,"moy"))	scorefn = moy_score;
	else {
		printf("Not recognised score %s\n", strscorefn);
		exit(1);
	}
	//
	Session *ses = load_ses(sesfile);
	Data *data = load_data(ses, datafile);
	//
	if (v_by_v) {
		show_result_v_by_v(ses, data, loss, scorefn);
	} else {
		show_result(ses, data, loss, scorefn);
	}
	//
	freeses(ses);
	freedata(ses, data);
};

int main(int argc, char* *argv) {
	if (argc == 2 && (!strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))) {
		printf("./bin/show_results.exe sesfile datafile loss score -params\n");
	} else if (argc >= 5) {
		char v_by_v=0;
		if (argc == 6)
			if (!strcmp(argv[5], "-v_by_v")) v_by_v = 1;
		show_results_(argv[1], argv[2], argv[3], argv[4], v_by_v);
	} else {
		printf("We need at least 4 arguments but got %i.\n", argc-1);
	}
	return 0;
}