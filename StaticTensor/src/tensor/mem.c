void _cmp_real_sizes(Tensor *ts) {
	ts->real_sizes[0] = 1;
	for (size_t i=1; i < ts->deep; i++)
		ts->real_sizes[i] = ts->real_sizes[i-1]*ts->shape[i-1];
};

Tensor* mkts(size_t deep, size_t *shape, double *arr, char type) {
	Tensor *ts = (Tensor*)malloc(sizeof(Tensor));
	//
	ts->deep = deep;
	ts->shape = (size_t*)malloc(deep*sizeof(size_t));
	ts->real_sizes = (size_t*)malloc(deep*sizeof(size_t));
	memcpy(ts->shape, shape, deep*sizeof(size_t));
	_cmp_real_sizes(ts);
	//
	ts->len = shape[deep-1]*ts->real_sizes[deep-1];
	ts->arr = (double*)malloc(ts->len * sizeof(double));
	memcpy(ts->arr, arr, ts->len*sizeof(double));
	//
	ts->type = type;
	return ts;
};

Tensor* cpyts(Tensor *ts) {
	Tensor *ret = (Tensor*)malloc(sizeof(Tensor));
	//
	ret->deep = ts->deep;
	//
	ret->shape = (size_t*)malloc(ret->deep * sizeof(size_t));
	//
	ret->real_sizes = (size_t*)malloc(ret->deep * sizeof(size_t));
	//
	for (size_t i=0; i < ret->deep; i++) {
		ret->shape[i] = ts->shape[i];
		ret->real_sizes[i] = ts->real_sizes[i];
	}
	//
	ret->len = ret->shape[ret->deep-1]*ret->real_sizes[ret->deep-1];
	ret->arr = (double*)malloc(ret->len * sizeof(double));
	for (size_t i=0; i < ret->len; i++)
		ret->arr[i] = ts->arr[i];
	//
	ret->type = ts->type;
	return ret;
};

void set_eq_ts(Tensor *dest, Tensor* src) {
	free(dest->arr);
	free(dest->shape);
	free(dest->real_sizes);
	//
	dest->deep = src->deep;
	dest->shape = (size_t*)malloc(dest->deep*sizeof(size_t));
	dest->real_sizes = (size_t*)malloc(dest->deep*sizeof(size_t));
	memcpy(dest->shape, src->shape, dest->deep*sizeof(size_t));
	_cmp_real_sizes(dest);
	//
	dest->len = dest->shape[dest->deep-1]*dest->real_sizes[dest->deep-1];
	dest->arr = (double*)malloc(dest->len * sizeof(double));
	memcpy(dest->arr, src->arr, dest->len*sizeof(double));
	//
	dest->type = src->type;
};

void freets(Tensor *ts) {
	free(ts->arr);
	free(ts->shape);
	free(ts->real_sizes);
	free(ts);
};