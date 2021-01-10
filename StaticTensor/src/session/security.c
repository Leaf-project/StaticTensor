void track_unused(Session *ses) {
	char where_computed[ses->vars];
	//
	size_t arg[6] = {0,0,0,0,0,0};	//six is juste max amount of arguments in one command
	size_t arg_pos = 0;
	//
	char chr;size_t pos=0;	//pos in code
	while (pos < ses->codelen) {
		chr = ses->code[pos++];
		if (chr < 10) arg[arg_pos] = arg[arg_pos]*10 + chr;
		else if (chr == 10) arg_pos++; //	, comma separate args
		else if (chr > 23) {	//23 is max
			printf("Can't recognize the chr : %i, with params %li %li %li %li %li %li.\nQuiting\n", chr, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);
			exit(1);
		}
		if (chr > 10) {
			where_computed[arg[0]] = 1;
			//
			for (size_t i=0; i < 6; i++) arg[i]=0;
			arg_pos = 0;
		}
	};
	//
	char isinput=0;
	for (size_t i=0; i < ses->vars; i++) {
		for (size_t j=0; j < ses->inputs; j++) if (i == ses->input[j]) isinput = 1;
		if (where_computed[i] == 0 && ses->var[0][i]->type == 0 && isinput == 0) {
			ses->var[0][i]->type = 3;	//directly set to unused
		}
		isinput=0;
	}
};