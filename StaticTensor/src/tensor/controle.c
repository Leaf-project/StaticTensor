size_t realpos(Tensor *ts, size_t *indexs) {
	size_t ret=0;
	for (size_t i=0; i < ts->deep; i++) ret += indexs[i]*ts->real_sizes[i];
	return ret;
};

void filltslist(Tensor *ts, double *value) {
	for (size_t i=0; i < ts->len; i++) ts->arr[i] = value[i];
};

void fillts(Tensor *ts, double value) {
	for (size_t i=0; i < ts->len; i++) ts->arr[i] = value;
};

void noisets(Tensor *ts) {
	for (size_t i=0; i <ts->len; i++) ts->arr[i] = rand_double();
};