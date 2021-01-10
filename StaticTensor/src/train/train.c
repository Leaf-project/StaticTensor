void _comp_time(Session *ses, Line *line, Loss loss, size_t time) {
	set_inputs(ses, line, time);
	forward(ses, time);
	//Errors
	set_errors(ses, line, loss, time);
	backward(ses, time);
};

void train_batch(Session* ses, Batch* batch, size_t bsize, Opti opti, Loss loss, Scorefn scorefn, double** *past_pass) {
	clear(ses);
	expand(ses);
	//
	for (size_t b=0; b < bsize; b++) {
		_comp_time(ses, batch->line[b], loss, b+1);
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
	//
	update(ses, opti, past_pass);
};

void train(Session* ses, Data* data, Opti opti, Loss loss, Scorefn scorefn, size_t echopes, char ptr) {
	double** * pass = mkpass(ses);
	double glob_loss;
	//ptrses(ses);
	for (size_t i=0; i < echopes; i++) {
		glob_loss = 0.0;	//here just for printing
		srand(i+1);
		for (size_t b=0; b < data->batchs; b++) {
			//Update
			train_batch(ses, data->batch[b], data->bsize, opti, loss, scorefn, pass);
			//Compute loss from grad 0
			for (size_t j=0; j < ses->outputs; j++)
				for (size_t k=0; k < ses->grad[0][ses->output[j]]->len; k++)
					glob_loss = fabs(loss(glob_loss, ses->grad[0][ses->output[j]]->arr[k]));
		}
		if (ptr) printf("Echope %li of %li with loss of %.6g\n", i, echopes, glob_loss);
	};
	freepass(pass, ses);
};

/*void train_only_badest(Session* ses, Data* data, Opti opti, size_t nb_pass, Loss loss, Scorefn scorefn, size_t echopes) {

};*/