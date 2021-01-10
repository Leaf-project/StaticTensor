void ptrdata(Session* ses, Data* data) {
	printf("Data\n");
	for (size_t i=0; i < data->batchs; i++) {
		printf("\033[104m|||||||||\033[0m \033[94mBatch\033[0m \033[36m#%li\033[0m \033[104m|||||||||\033[0m\n", i);
		printf("Score : %f\n", data->batch[i]->score);
		for (size_t j=0; j < data->bsize; j++) {
			printf("\033[107m===\033[0m Line \033[30m%li\033[0m of batch \033[30m%li\033[0m \033[107m===\033[0m\n", j, i);
			printf("\033[100mInputs\033[0m\n");
			for (size_t k=0; k < ses->inputs; k++) ptrts(data->batch[i]->line[j]->ts[k]);
			printf("\033[100mOutputs\033[0m\n");
			for (size_t k=0; k < ses->inputs; k++) ptrts(data->batch[i]->line[j]->ts[ses->inputs + k]);
		}
	}
};

void show_result(Session *ses, Data *data, Loss loss, Scorefn scorefn) {
	compute_all_scores(ses, data, loss, scorefn);
	//Print
	for (size_t i=0; i < data->batchs; i++) {
		printf("========Batch %li with score of %.4g=======\n", i, data->batch[i]->score);
		compute_score(ses, data->batch[i], loss, data->bsize, scorefn);
		//Print corrctly results
		for (size_t t=0; t < ses->times; t++) {
			printf("|||||||TIME %li (batch)|||||||\n", t);
			printf("Inputs\n");
			for (size_t j=0; j < ses->inputs; j++) {
				ptrts(ses->var[t][ses->input[j]]);
			};
			printf("Outputs\n");
			for (size_t j=0; j < ses->outputs; j++) {
				ptrts(ses->var[t][ses->output[j]]);
			};
			printf("Wants\n");
			for (size_t j=0; j < ses->outputs; j++) {
				ptrts(data->batch[i]->line[t]->ts[ses->inputs + j]);
			}
			printf("Output grads (errors)\n");
			for (size_t j=0; j < ses->outputs; j++) {
				ptrts(ses->grad[t][ses->output[j]]);
			};
		}
	}
};

void show_result_v_by_v(Session *ses, Data *data, Loss loss, Scorefn scorefn) {
	//compute_all_scores(ses, data, loss, scorefn);
	//Print
	for (size_t i=0; i < data->batchs; i++) {
		compute_score(ses, data->batch[i], loss, data->bsize, scorefn);
		printf("========Batch %li with score of %.3g=======\n", i, data->batch[i]->score);
		//Print corrctly results
		for (size_t t=1; t < ses->times; t++) {	//time is not realy time, it's batch time
			printf("|||||||TIME %li (batch)|||||||\n", t-1);
			for (size_t j=0; j < ses->outputs; j++) {
				printf("Output #%li\n", j);
				for (size_t k=0; k < ses->var[0][ses->output[j]]->len; k++) {
					printf("Get:\033[96m%.4g\033[0m Want:\033[92m%.3g\033[0m Loss:\033[91m%.3g\033[0m\n", ses->var[t][ses->output[j]]->arr[k], data->batch[i]->line[t-1]->ts[ses->inputs + j]->arr[k], ses->grad[t][ses->output[j]]->arr[k]);
				}
			}
		}
	}
};