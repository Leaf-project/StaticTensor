double _sgd(double w, double dw, double* pass) {
	return w + dw;
};

double _momentum(double w, double dw, double* pass) {
	pass[0] = 0.9*pass[0] + 0.1*dw;
	return w + pass[0];
};

double _rmsprop(double w, double dw, double* pass) {
	pass[0] = 0.9*pass[0] + 0.1*pow(dw, 2);
	return w + 0.1*dw/(pow(pass[0], 0.5) + 1e-10);
};

double _adagrad(double w, double dw, double* pass) {
	pass[0] = pass[0] + pow(dw, 2);
	return w + 0.1*dw/(pow(pass[0], 0.5) + 1e-10);
};

double _adam(double w, double dw, double* pass) {
	pass[0] = 0.9*pass[0] - 0.1*dw;			//v[t] = (b[1] * v[t-1]) - (1-b[1])*dw
	pass[1] = 0.99*pass[1] - 0.01*pow(dw, 2);	//s[t] = (b[1] * s[t-1]) - (1-b[1])*dw**2
	return w + 0.1*pass[0]*dw/(pow(pass[1], 0.5) + 1e-10);
};

Tensor* MDT(Tensor* w, Tensor* dw, double** pass, double (*mdt)(double,double,double*)) {
	Tensor* ret = cpyts(w);
	for (size_t i=0; i < ret->len; i++)
		ret->arr[i] = mdt(w->arr[i], dw->arr[i], pass[i]);
	return ret;
};

Tensor* Sgd(Tensor *w, Tensor *dw, double **pass) {return MDT(w,dw,pass,&_sgd);};
Tensor* Momentum(Tensor *w, Tensor *dw, double **pass) {return MDT(w,dw,pass,&_momentum);};
Tensor* RMSProp(Tensor *w, Tensor *dw, double **pass) {return MDT(w,dw,pass,&_rmsprop);};
Tensor* AdaGrad(Tensor *w, Tensor *dw, double **pass) {return MDT(w,dw,pass,&_adagrad);};
Tensor* Adam(Tensor *w, Tensor *dw, double **pass) {return MDT(w,dw,pass,&_adam);};

double*** mkpass(Session *ses, size_t nb_pass) {
	double** *pass = (double***)malloc(ses->vars * sizeof(double**));
	for (size_t i=0; i < ses->vars; i++) {
		pass[i] = (double**)malloc(ses->var[0][i]->len * sizeof(double*));
		for (size_t j=0; j < ses->var[0][i]->len; j++) {
			pass[i][j] = (double*)malloc(nb_pass * sizeof(double));
		}
	}
	return pass;
};

void freepass(double** *pass, Session *ses, size_t nb_pass) {
	for (size_t i=0; i < ses->vars; i++) {
		for (size_t j=0; j < ses->var[0][i]->len; j++) {
			free(pass[i][j]);
		}
		free(pass[i]);
	}
	free(pass);
};