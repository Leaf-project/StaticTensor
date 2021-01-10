void writets(Tensor *ts, FILE *fp) {
	fwrite(&ts->deep, sizeof(size_t), 1, fp);
	fwrite(ts->shape, sizeof(size_t), ts->deep, fp);
	fwrite(ts->arr, sizeof(double), ts->len, fp);
	fwrite(&ts->type, sizeof(char), 1, fp);
};

Tensor* readts(FILE *fp) {
	Tensor* ret = (Tensor*)malloc(sizeof(Tensor));
	//
	fread(&ret->deep, sizeof(size_t), 1, fp);
	//
	ret->shape = (size_t*)malloc(ret->deep * sizeof(size_t));
	fread(ret->shape, sizeof(size_t), ret->deep, fp);
	//
	ret->real_sizes = (size_t*)malloc(ret->deep * sizeof(size_t));
	_cmp_real_sizes(ret);
	//
	ret->len = ret->shape[ret->deep-1] * ret->real_sizes[ret->deep-1];
	ret->arr = (double*)malloc(ret->len * sizeof(double));
	fread(ret->arr, sizeof(double), ret->len, fp);
	//
	fread(&ret->type, sizeof(char), 1, fp);
	//
	return ret;
};