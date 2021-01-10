void set_inputs(Session *ses, Line *line, size_t time) {
	for (size_t i=0; i < ses->inputs; i++)
		ses->var[time][ses->input[i]] = cpyts(line->ts[i]);
};

void set_errors(Session *ses, Line *line, Loss loss, size_t time) {
	for (size_t i=0; i < ses->outputs; i++) {
		//ses->grad[time][ses->output[i]] = cpyts(ses->var[0][ses->output[i]]);
		for (size_t j=0; j < ses->grad[time][ses->output[i]]->len; j++) {
			ses->grad[time][ses->output[i]]->arr[j] = loss(
				line->ts[ses->inputs + i]->arr[j],
				ses->var[time][ses->output[i]]->arr[j]
			);
		}
	}
};

void compute_time(Session *ses, Line *line, Loss loss, size_t time) {
	set_inputs(ses, line, time);
	forward(ses, time);
	//Errors	//just to have some gradients
	set_errors(ses, line, loss, time);
};

void compute_score(Session *ses, Batch *batch, Loss loss, size_t bsize, Scorefn scorefn) {
	clear(ses);
	expand(ses);
	for (size_t b=0; b < bsize; b++) {
		compute_time(ses, batch->line[b], loss, b+1);
		if (b+1 < bsize) expand(ses);
	}
	//Sum grads
	sum_grads(ses);
	//Set score
	double *lst = (double*)malloc(ses->vars * sizeof(double));
	for (size_t i=0; i < ses->vars; i++)
		for (size_t j=0; j < ses->var[0][i]->len; j++)
			lst[i] += ses->grad[0][i]->arr[j];
	batch->score = scorefn(lst, ses->vars);
	free(lst);
};

void compute_all_scores(Session *ses, Data *data, Loss loss, Scorefn scorefn) {
	for (size_t i=0; i < data->batchs; i++)
		compute_score(ses, data->batch[i], loss, data->bsize, scorefn);
};

char sorted(Data *data, size_t *list, size_t len) {
	for (size_t i=0; i < len-1; i++)
		if (data->batch[list[i]]->score < data->batch[list[i+1]]->score)
			return 0;
	return 1;
};

void sort_data(Data *data) {	//better to worst
	size_t c;
	//
	size_t *list = (size_t*)malloc(data->batchs * sizeof(size_t));
	for (size_t i=0; i < data->batchs; i++) list[i] = i;
	//Sort
	while (!sorted(data, list, data->batchs)) {
		for (size_t i=0; i < data->batchs-1; i++) {
			if (data->batch[list[i]]->score > data->batch[list[i+1]]->score) {
				c = list[i];
				list[i] = list[i+1];
				list[i+1] = c;
			}
		}
	}
	//
	Batch **batch = (Batch**)malloc((data->batchs) * sizeof(Batch*));
	for (size_t i=0; i < data->batchs; i++) batch[i] = data->batch[i];
	//Replace
	for (size_t i=0; i < data->batchs; i++) data->batch[i] = batch[list[i]];
	//
	free(batch);
};