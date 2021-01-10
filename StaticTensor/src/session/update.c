void _update_var(Session *ses, Opti opti, double* *pass, size_t var) {
	Tensor *ts = opti(ses->var[0][var], ses->grad[0][var], pass);
	freets(ses->var[0][var]);
	ses->var[0][var] = ts;
};

void update(Session *ses, Opti opti, double** *pass) {
	for (size_t i=0; i < ses->vars; i++)
		if (ses->var[0][i]->type == 1)
			_update_var(ses, opti, pass[i], i);
};