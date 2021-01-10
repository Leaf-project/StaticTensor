char is_in(size_t i, size_t* lst, size_t len) {
	for (size_t j=0; j < len; j++) if (i == lst[j]) return 1;
	return 0;
};

void ptrvar(Session *ses) {
	char*** strss = (char***)malloc(ses->times * sizeof(char**));
	size_t lengths;
	//
	printf("\033[100mVARIABLES\033[0m\n");
	for (size_t i=0; i < ses->vars; i++) {
		printf("\033[93m#%s\033[0m \033[30m%li\033[0m", str_types[(size_t)ses->var[0][i]->type], i);
		//
		if (is_in(i, ses->input, ses->inputs)) printf(" \033[100minput\033[0m\n");
		else if (is_in(i, ses->output, ses->outputs)) printf(" \033[100moutput\033[0m\n");
		else printf("\n");
		//
		//debbug_ptrmat(ses->var[0][i]);
		for (size_t t=0; t < ses->times; t++) {
			strss[t] = strts(ses->var[t][i]);
		}
		lengths = lineslen(strss[0]);
		//
		for (size_t j=0; j < lengths; j++) {
			for (size_t t=0; t < ses->times; t++) {
				printf("%s|", strss[t][j]);
			}
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			for (size_t j=0; j < lengths; j++) {
				free(strss[t][j]);
			}
			free(strss[t]);
		}
	}
	free(strss);
};

void ptrgrad(Session *ses) {
	char*** strss = (char***)malloc(ses->times * sizeof(char**));
	size_t lengths;
	//
	printf("\033[100mGRADIENTS\033[0m\n");
	for (size_t i=0; i < ses->vars; i++) {
		printf("\033[91m#%s\033[0m \033[30m%li\033[0m", str_types[(size_t)ses->grad[0][i]->type], i);
		//
		if (is_in(i, ses->input, ses->inputs)) {
			printf(" \033[100minput\033[0m\n");
		} else if (is_in(i, ses->output, ses->outputs)) {
			printf(" \033[100moutput\033[0m\n");
		} else {
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			strss[t] = strts(ses->grad[t][i]);
		}
		lengths = lineslen(strss[0]);
		//
		for (size_t j=0; j < lengths; j++) {
			for (size_t t=0; t < ses->times; t++) {
				printf("%s|", strss[t][j]);
			}
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			for (size_t j=0; j < lengths; j++) {
				free(strss[t][j]);
			}
			free(strss[t]);
		}
	}
	free(strss);
};

void ptrcode(Session *ses) {
	//Code
	printf("\033[100mCODE\033[0m\n");
	//Print out code
	size_t pos=0,tmp_pos_save=0;
	//
	size_t arg[6] = {0,0,0,0,0,0};
	size_t arg_pos = 0;
	//
	char chr;
	while (pos < ses->codelen) {
		chr = ses->code[pos++];
		if (chr > 10) {
			printf("%li|", tmp_pos_save);
			tmp_pos_save = pos;
		}
		if (chr < 10) {
			arg[arg_pos] = arg[arg_pos]*10 + chr;
		} else if (chr == 10) {			//	,
			arg_pos++;
		} else if (11 <= chr && chr <= 15) {	//	+ - * / ^
			printf("$%li[\033[2m-%li\033[0m] = $%li[\033[2m-%li\033[0m] \033[94m%s\033[0m $%li[\033[2m-%li\033[0m]", arg[0], arg[1]+1, arg[2], arg[3]+1, str_ops[chr-11], arg[4], arg[5]+1);
			printf(" | ");ptrtsshape(ses->var[0][arg[0]]);printf(" = ");ptrtsshape(ses->var[0][arg[2]]);printf(" %s ", str_ops[chr-11]);ptrtsshape(ses->var[0][arg[4]]);printf("\n");
		} else if (chr == 16) {			// @
			printf("$%li[\033[2m-%li\033[0m] = $%li[\033[2m-%li\033[0m] \033[94m@\033[0m $%li[\033[2m-%li\033[0m]", arg[0], arg[1]+1, arg[2], arg[3]+1, arg[4], arg[5]+1);
			printf(" | ");ptrtsshape(ses->var[0][arg[0]]);printf(" = ");ptrtsshape(ses->var[0][arg[2]]);printf(" @ ");ptrtsshape(ses->var[0][arg[4]]);printf("\n");
		} else if (17 <= chr && chr <= 21) {	//	iter + - * / ^
			printf("$%li[\033[2m-%li\033[0m] = iter \033[94m%s\033[0m ($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, str_ops[chr-17], arg[2], arg[3]+1);
		} else if (chr == 22) {			//	T
			printf("$%li[\033[2m-%li\033[0m] = transpose($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, arg[2], arg[3]+1);
		} else if (chr == 23) {		//	)	from logistic, tanh, ReLU ...
			printf("$%li[\033[2m-%li\033[0m] = \033[96m%s\033[0m($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, str_funcs[arg[4]], arg[2], arg[3]+1);
		} else {
			printf("Arguments (%li %li %li %li %li %li)(arg_pos:%li) unknown character : %i\n", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg_pos, chr);
		}
		if (chr > 10) {
			for (size_t i=0; i < 6; i++) arg[i] = 0;
			arg_pos = 0;
		}
	}
	printf("\033[100mPrint out brut code\033[0m\n");
	for (size_t i=0; i < ses->codelen; i++) {
		printf("\033[40m%li\033[0m:%i\033[31m|\033[0m", i, ses->code[i]);
	}
	printf("\n");
};

void ptrses(Session* ses) {
	ptrvar(ses);
	ptrgrad(ses);
	ptrcode(ses);
};

/*
void ptrses(Session* ses) {
	char*** strss = (char***)malloc(ses->times * sizeof(char**));
	size_t lengths;
	//
	printf("\033[100mVARIABLES\033[0m\n");
	for (size_t i=0; i < ses->vars; i++) {
		printf("\033[93m#%s\033[0m \033[30m%li\033[0m", str_types[ses->var[0][i]->type], i);
		//
		if (is_in(i, ses->input, ses->inputs)) printf(" \033[100minput\033[0m\n");
		else if (is_in(i, ses->output, ses->outputs)) printf(" \033[100moutput\033[0m\n");
		else printf("\n");
		//
		//debbug_ptrmat(ses->var[0][i]);
		for (size_t t=0; t < ses->times; t++) {
			strss[t] = strts(ses->var[t][i]);
		}
		lengths = lineslen(strss[0]);
		//
		for (size_t j=0; j < lengths; j++) {
			for (size_t t=0; t < ses->times; t++) {
				printf("%s|", strss[t][j]);
			}
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			for (size_t j=0; j < lengths; j++) {
				free(strss[t][j]);
			}
			free(strss[t]);
		}
	}
	//Gradients
	printf("\033[100mGRADIENTS\033[0m\n");
	for (size_t i=0; i < ses->vars; i++) {
		printf("\033[91m#%s\033[0m \033[30m%li\033[0m", str_types[ses->grad[0][i]->type], i);
		//
		if (is_in(i, ses->input, ses->inputs)) {
			printf(" \033[100minput\033[0m\n");
		} else if (is_in(i, ses->output, ses->outputs)) {
			printf(" \033[100moutput\033[0m\n");
		} else {
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			strss[t] = strts(ses->grad[t][i]);
		}
		lengths = lineslen(strss[0]);
		//
		for (size_t j=0; j < lengths; j++) {
			for (size_t t=0; t < ses->times; t++) {
				printf("%s|", strss[t][j]);
			}
			printf("\n");
		}
		//
		for (size_t t=0; t < ses->times; t++) {
			for (size_t j=0; j < lengths; j++) {
				free(strss[t][j]);
			}
			free(strss[t]);
		}
	}
	//Code
	printf("\033[100mCODE\033[0m\n");
	//Print out code
	size_t pos=0,tmp_pos_save=0;
	//
	size_t arg[6] = {0,0,0,0,0,0};
	size_t arg_pos = 0;
	//
	char chr;
	while (pos < ses->codelen) {
		chr = ses->code[pos++];
		if (chr > 10) {
			printf("%li|", tmp_pos_save);
			tmp_pos_save = pos;
		}
		if (chr < 10) {
			arg[arg_pos] = arg[arg_pos]*10 + chr;
		} else if (chr == 10) {			//	,
			arg_pos++;
		} else if (11 <= chr && chr <= 15) {	//	+ - * / ^
			printf("$%li[\033[2m-%li\033[0m] = $%li[\033[2m-%li\033[0m] \033[94m%s\033[0m $%li[\033[2m-%li\033[0m]\n", arg[0], arg[1]+1, arg[2], arg[3]+1, str_ops[chr-11], arg[4], arg[5]+1);
		} else if (chr == 16) {			// @
			printf("$%li[\033[2m-%li\033[0m] = $%li[\033[2m-%li\033[0m] \033[94m@\033[0m $%li[\033[2m-%li\033[0m]\n", arg[0], arg[1]+1, arg[2], arg[3]+1, arg[4], arg[5]+1);
		} else if (17 <= chr && chr <= 21) {	//	iter + - * / ^
			printf("$%li[\033[2m-%li\033[0m] = iter \033[94m%s\033[0m ($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, str_ops[chr-17], arg[2], arg[3]+1);
		} else if (chr == 22) {			//	T
			printf("$%li[\033[2m-%li\033[0m] = transpose($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, arg[2], arg[3]+1);
		} else if (chr == 23) {		//	)	from logistic, tanh, ReLU ...
			printf("$%li[\033[2m-%li\033[0m] = \033[96m%s\033[0m($%li[\033[2m-%li\033[0m])\n", arg[0], arg[1]+1, str_funcs[arg[4]], arg[2], arg[3]+1);
		} else {
			printf("Arguments (%li %li %li %li %li %li)(arg_pos:%li) unknown character : %i\n", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg_pos, chr);
		}
		if (chr > 10) {
			for (size_t i=0; i < 6; i++) arg[i] = 0;
			arg_pos = 0;
		}
	}
	printf("\033[100mPrint out brut code\033[0m\n");
	for (size_t i=0; i < ses->codelen; i++) {
		printf("\033[40m%li\033[0m:%i\033[31m|\033[0m", i, ses->code[i]);
	}
	printf("\n");
};
*/