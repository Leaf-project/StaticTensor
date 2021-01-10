#include <stdio.h>

#include "../include/train.h"

/*
./bin/normal_train.exe sesfile datafile sesfiletosave echopes methode loss score
*/

void normal_train_(char *fses, char *fdata, char *fsessave, size_t echopes, char *sopti, char *sloss, char *sscore, char ptr) {
	Opti opti;
	if (!strcmp(sopti,"default"))		opti = Sgd;
	else if (!strcmp(sopti,"Sgd"))		opti = Sgd;
	else if (!strcmp(sopti,"Momentum"))	opti = Momentum;
	else if (!strcmp(sopti,"RMSProp"))	opti = RMSProp;
	else if (!strcmp(sopti,"AdaGrad"))	opti = AdaGrad;
	else if (!strcmp(sopti,"Adam"))		opti = Adam;
	else {
		printf("Not recognised methode %s\n", sopti);
		exit(1);
	}
	//Loss
	Loss loss;
	if (!strcmp(sloss,"default"))	loss = mbe;
	else if (!strcmp(sloss,"mbe"))	loss = mbe;
	else if (!strcmp(sloss,"mse"))	loss = mse;
	else if (!strcmp(sloss,"mae"))	loss = mae;
	else if (!strcmp(sloss,"cel"))	loss = cel;
	else {
		printf("Not recognised loss %s\n", sloss);
		exit(1);
	}
	//Score
	Scorefn scorefn;
	if (!strcmp(sscore,"default"))	scorefn = sum_score;
	else if (!strcmp(sscore,"sum"))	scorefn = sum_score;
	else if (!strcmp(sscore,"moy"))	scorefn = moy_score;
	else {
		printf("Not recognised score %s\n", sscore);
		exit(1);
	}
	//
	Session *ses = load_ses(fses);
	Data *data = load_data(ses, fdata);
	//because 2 is max pass
	train(ses, data, opti, loss, scorefn, echopes, ptr);
	save_ses(ses, fsessave);
	//
	freedata(ses, data);
	freeses(ses);
};

int main(int argc, char* *argv) {
	if (argc == 2 && (!strcmp(argv[1],"help") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))) {
		printf("./bin/normal_train.exe sesfile datafile sesfiletosave echopes methode loss_fn score_fn\n");
	} else if (argc >= 8) {
		char ptr=0;
		if (argc > 8) {
			if (!strcmp(argv[8], "-ptr")) ptr=1;
		}
		normal_train_(argv[1], argv[2], argv[3], atoi(argv[4]), argv[5], argv[6], argv[7], ptr);
	} else {
		printf("We need 7 arguments but got %i.\n", argc-1);
	}
	return 0;
}