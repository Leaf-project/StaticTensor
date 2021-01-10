char same_shapes(Tensor *a, Tensor *b) {
	if (a->deep != b->deep) return 0;
	for (size_t i=0; i < a->deep; i++) if (a->shape[i] != b->shape[i]) return 0;
	return 1;
};

Tensor* operator(Tensor *a, Tensor *b, Op op) {
	Tensor *ret = cpyts(a);
	if (same_shapes(a, b)) {
		for (size_t i=0; i < a->len; i++) {
			ret->arr[i] = op(a->arr[i], b->arr[i]);
		}
	} else if (a->len == 1) {
		for (size_t i=0; i < b->len; i++)
			ret->arr[i] = op(a->arr[0], b->arr[i]);
	} else if (b->len == 1) {
		for (size_t i=0; i < a->len; i++)
			ret->arr[i] = op(a->arr[i], b->arr[0]);
	} else {
		printf("Operator not same shape.\n");
		freets(ret);
		exit(1);
	}
	return ret;
};

Tensor* iter(Tensor *ts, Op op) {
	size_t shape[] = {1};
	double arr[1];arr[0]=0.0;
	for (size_t i=0; i < ts->len; i++) arr[0] = op(arr[0], ts->arr[i]);
	Tensor *ret = mkts(1, shape, arr, ts->type);
	return ret;
};

Tensor* activate(Tensor *ts, Fn fn) {
	Tensor *ret = cpyts(ts);
	for (size_t i=0; i < ts->len; i++) ret->arr[i] = fn(ts->arr[i]);
	return ret;
};

Tensor* transpose(Tensor *a) {
	//Temporaty
	int i;size_t j;
	//Mk tensor with inversed shape
	Tensor *ret = (Tensor*)malloc(sizeof(Tensor));
	ret->deep = a->deep;
	ret->shape = (size_t*)malloc(ret->deep * sizeof(size_t));
	ret->real_sizes = (size_t*)malloc(ret->deep * sizeof(size_t));
	for (j=0; j < a->deep; j++) ret->shape[j] = a->shape[a->deep-j-1]; //Inverse shape
	_cmp_real_sizes(ret);
	ret->len = ret->real_sizes[ret->deep-1] * ret->shape[ret->deep-1];
	ret->arr = (double*)malloc(ret->len * sizeof(double));
	ret->type = 0; //because change
	//indexes
	size_t *iters = (size_t*)malloc(ret->deep * sizeof(size_t));	//Reversed
	size_t *invers_iters = (size_t*)malloc(a->deep * sizeof(size_t));
	//
	for (j=0; j < ret->deep; j++) {
		iters[j]=0;invers_iters[j]=0;
	}
	//
	char loop = 1;
	//
	while (loop) {
		//inverse iter
		for (j=0; j < ret->deep; j++) invers_iters[ret->deep-j-1] = iters[j];
		//
		ret->arr[realpos(ret, invers_iters)] = a->arr[realpos(a, iters)];	//Transposed pos
		//Update iters (+1)
		i = a->deep;		//a->deep-1  (like i=0)
		//Loop to make +1 to lst (with limits on each case)
		while (--i >= 0) {	//i >= 0; to the end of while add --i;
			iters[i] += 1;
			if (iters[i] != a->shape[i]) break;	//Chech is under limite (<)
			else if (i == 0) loop = 0;
			else iters[i] = 0;
		}
	}
	return ret;
};

char _ok_dot(Tensor* a, Tensor* b) {
	if (a->shape[0] != b->shape[1]) return 0;
	if (a->deep > 2) for (size_t i=2; i < a->deep; i++) if (a->shape[i] != b->shape[i]) return 0;
	return 1;
};

Tensor* dot(Tensor* a, Tensor* b) {
	Tensor* ret;
	if (a->deep == 1 && b->deep == 1) {
		//Spetial functions
		if (a->shape[0] == b->shape[0]) {
			size_t shape[] = {1};
			double arr[1] = {0.0};
			ret = mkts(1, shape, arr, 0);
			for (size_t i=0; i < a->len; i++) ret->arr[0] += a->arr[i] * b->arr[i];
		} else {
			printf("Dot product, a->deep == b->deep == 1, shapes : %li %li\n", a->shape[0], b->shape[1]);
			exit(1);
		}
	} else if (a->deep != b->deep) {
		printf("Dot product not same deep : %li != %li\n", a->deep, b->deep);
		exit(1);
	} else if (_ok_dot(a, b)) {
		size_t tmp_;	//tmp_ is just here to make a,b = b,a (tmp_ is c in :c=a;a=b;b=c)	(but to a,b=b,a we can make (useless x2 slower) : a=a-b;b=a+b;a=-a+b)
		size_t j;
		double tmp0, tmp1;	//elm in line or col in mat_a and mat_b
		//
		size_t* shape = (size_t*)malloc(a->deep * sizeof(size_t));
		for (j=0; j < a->deep; j++) shape[j] = b->shape[j];
		shape[1] = a->shape[1];
		//
		size_t len = mul(shape, a->deep);
		double *arr = (double*)malloc(len * sizeof(double));
		for (j=0; j < len; j++) arr[j] = 0.0;
		ret = mkts(a->deep, shape, arr, 0);
		//
		size_t* iters = (size_t*)malloc(a->deep * sizeof(size_t));
		for (j=0; j < a->deep; j++) iters[j] = 0;
		//
		char loop = 1;
		while (loop) {	//for each in ret
			/* Algo: make normal 2-dim dot product but repeat at highter layer */
			for (j=0; j < a->shape[0]; j++) {
				//get tmp0 from a
				tmp_ = iters[0];
				iters[0] = j;
				tmp0 = a->arr[realpos(a, iters)];
				iters[0] = tmp_;
				//get tmp1 from b
				tmp_ = iters[1];
				iters[1] = j;
				tmp1 = b->arr[realpos(b, iters)];
				iters[1] = tmp_;
				//Add
				ret->arr[realpos(ret, iters)] += tmp0 * tmp1;
			}
			//
			for (j=0; j < a->deep; j++) {
				iters[j]++;
				if (iters[j] != ret->shape[j]) break;
				else if (j == a->deep-1) loop = 0;
				else iters[j] = 0;
			}
		};
	} else {
		printf("Dot correct deeps but Invalid shapes deep:%li and a->shape[0] != b->shape[1] (%li != %li)\n", a->deep, a->shape[0], b->shape[1]);
		exit(1);
	}
	return ret;
};