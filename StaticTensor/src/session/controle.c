void rnd_noise_cells(Session *ses, size_t t) {
	for (size_t i=0; i < ses->vars; i++) {
		if (ses->var[0][i]->type == 4) {	//noise tensor
			noisets(ses->var[t][i]);
		}
	}
};

void restart(Session *ses, size_t time) {
	for (size_t i=0; i < ses->vars; i++)
		if (ses->var[time][i]->type == 0)
			freets(ses->var[time][i]);
	for (size_t i=0; i < ses->vars; i++)
		freets(ses->grad[time][i]);
};

void clear(Session *ses) {
	if (ses->times != 1) {
		for (size_t t=1; t < ses->times; t++) {
			for (size_t i=0; i < ses->vars; i++) {
				freets(ses->var[t][i]);
				freets(ses->grad[t][i]);
			}
			//
			free(ses->var[t]);
			free(ses->grad[t]);
		}
		//
		ses->var = (Tensor***)realloc(ses->var, 1*sizeof(Tensor**));
		ses->grad = (Tensor***)realloc(ses->grad, 1*sizeof(Tensor**));
		ses->times = 1;
	}
};

void expand(Session* ses) {
	ses->var = (Tensor***)realloc(ses->var, (++ses->times) * sizeof(Tensor**));
	ses->grad = (Tensor***)realloc(ses->grad, (ses->times) * sizeof(Tensor**));
	//
	ses->var[ses->times - 1] = (Tensor**)malloc(ses->vars * sizeof(Tensor*));
	ses->grad[ses->times - 1] = (Tensor**)malloc(ses->vars * sizeof(Tensor*));
	//
	for (size_t i=0; i < ses->vars; i++) {
		//Vars
		if (ses->var[0][i]->type != 0) {		//Cpy
			ses->var[ses->times-1][i] = cpyts(ses->var[0][i]);
		}
		//Creat Gradient Tensor set to zero
		ses->grad[ses->times-1][i] = cpyts(ses->grad[0][i]);
		fillts(ses->grad[ses->times-1][i], 0.0);
	}
};

void sum_grads(Session* ses) {
	//Clearning time=0
	for (size_t i=0; i < ses->vars; i++)
		fillts(ses->grad[0][i], 0.0);
	//Summing all
	for (size_t t=1; t < ses->times; t++)
		for (size_t i=0; i < ses->vars; i++)
			for (size_t j=0; j < ses->grad[0][i]->len; j++)
				ses->grad[0][i]->arr[j] += ses->grad[t][i]->arr[j];
};