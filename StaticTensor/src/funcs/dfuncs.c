double d_identity(double x) {
	(void)x;
	return 1.0;
};

double d_xor_fn(double x) {
	return 400*x / pow(pow(x, 2) + 100, 2);
};

double d_bin_step(double x) {
	(void)x;
	return 0.0;
};

double d_logistic(double x) {
	double a = _logistic(x);
	return a*(1 - a);
};

double d_tanh(double x) {
	return 1.0 - pow(tanh(x), 2);
};

double d_ReLU(double x) {
	if (x < 0) return 0;
	else return 1;
};

double d_GELU(double x) {
	double tmp0 = tanh(pow(2,0.5) * (0.044715*pow(x,3))/pow(M_PI, 0.5));
	return (pow(2,0.5)*x*(1-pow(tmp0, 2))*(0.134145*pow(x, 2) + 1)/pow(M_PI, 0.5) + tmp0 + 1) / 2;
};

double d_SoftPlus(double x) {
	return 1 / (1 + exp(-x));
};

double d_ELU(double x) {
	double a=1;
	if (x < 0) return a*exp(x);
	else return 1;
};

double d_SELU(double x) {
	double lambda=1.0507, alpha=1.67326;
	if (x < 0) return lambda * alpha * exp(x);
	else return lambda;
};

double d_Leaky_ReLU(double x) {
	if (x < 0) return 0.01;
	else return 1;
};

double d_Softsign(double x) {
	return 1 / pow(1 + fabs(x), 2);
};

double d_SQNL(double x) {
	return 1 + x/2;
};

double d_Bent_Ind(double x) {
	return x / (2 * pow(pow(x, 2)+1, 0.5)) + 1;
};

double d_Gauss(double x) {
	return -2*x*exp(-pow(x, 2));
};

double d_SQ_RBF(double x) {
	if (fabs(x) <= 1) return -x;
	else if (1 < fabs(x) && fabs(x) < 2) return x - 2*sign(x);
	else return 0;
};

//
Tensor* didentity(Tensor* x)	{return func(x, &d_identity);};
Tensor* dxor_fn(Tensor *x)		{return func(x, &d_xor_fn);};
Tensor* dbin_step(Tensor* x)	{return func(x, &d_bin_step);};
Tensor* dLogistic(Tensor* x)	{return func(x, &d_logistic);};
Tensor* dTanh(Tensor* x) 		{return func(x, &d_tanh);};
Tensor* dReLU(Tensor* x)		{return func(x, &d_ReLU);};
Tensor* dGELU(Tensor* x)		{return func(x, &d_GELU);};
Tensor* dSoftPlus(Tensor* x)	{return func(x, &d_SoftPlus);};
Tensor* dELU(Tensor* x)			{return func(x, &d_ELU);};
Tensor* dSELU(Tensor* x)		{return func(x, &d_SELU);};
Tensor* dLeaky_ReLU(Tensor* x)	{return func(x, &d_Leaky_ReLU);};
Tensor* dSoftsign(Tensor* x)	{return func(x, &d_Softsign);};
Tensor* dSQNL(Tensor* x)		{return func(x, &d_SQNL);};
Tensor* dBent_Ind(Tensor* x)	{return func(x, &d_Bent_Ind);};
Tensor* dGauss(Tensor* x)		{return func(x, &d_Gauss);};
Tensor* dSQ_RBF(Tensor* x)		{return func(x, &d_SQ_RBF);};
//
Tensor* dSoftMax(Tensor* x) {
	/*
	def softmax(arr):
		arr = np.exp(arr - max(arr))
		return arr / sum(arr)

	def dsoftmax(arr):
		arr = np.exp(arr - max(arr))
		sum = sum(arr)
		return arr * (sum - arr) / sum**2
	*/
	double *lst = (double*)malloc(x->len * sizeof(double));
	//
	size_t i;
	double tmp = 0.0;	//here max
	for (i=0; i < x->len; i++) if (x->arr[i] > tmp) tmp = x->arr[i];
	//
	for (i=0; i < x->len; i++) lst[i] = exp(x->arr[i]-tmp);
	//	here sum
	tmp = 0.0;
	for (i=0; i < x->len; i++) tmp += lst[i];
	for (i=0; i < x->len; i++) lst[i]  = lst[i] * (tmp - lst[i]) / pow(tmp, 2);
	//
	Tensor *ret = cpyts(x);
	filltslist(ret, lst);
	free(lst);
	return ret;
};

Tensor* dMaxout(Tensor *x) {
	size_t i;
	double* lst = (double*)malloc(x->len * sizeof(double));
	size_t max=0;	//here max
	for (i=0; i < x->len; i++) if (x->arr[i] > max) max = x->arr[i];
	//
	for (i=0; i < x->len; i++) {
		if (i == max) lst[i] = 1.0;
		else lst[i] = 0.0;
	};
	Tensor *ret = cpyts(x);
	filltslist(ret, lst);
	free(lst);
	return ret;
};