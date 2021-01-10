Data* load_data(Session *ses, char *file) {
	FILE *fp = fopen(file, "rb");
	//
	Data* ret = (Data*)malloc(sizeof(Data));
	//
	fread(&ret->bsize, sizeof(size_t), 1, fp);
	fread(&ret->batchs, sizeof(size_t), 1, fp);
	//
	ret->batch = (Batch**)malloc(ret->batchs * sizeof(Batch*));
	//
	for (size_t i=0; i < ret->batchs; i++) {
		ret->batch[i] = (Batch*)malloc(sizeof(Batch));
		ret->batch[i]->line = (Line**)malloc(ret->bsize * sizeof(Line*));
		for (size_t j=0; j < ret->bsize; j++) {
			ret->batch[i]->line[j] = (Line*)malloc(sizeof(Line));
			ret->batch[i]->line[j]->ts = (Tensor**)malloc((ses->inputs + ses->outputs) * sizeof(Tensor*));
			for (size_t k=0; k < (ses->inputs + ses->outputs); k++) {
				ret->batch[i]->line[j]->ts[k] = readts(fp);
			}
		}
	}
	//
	return ret;
};

void freedata(Session *ses, Data *data) {
	for (size_t b=0; b < data->batchs; b++) {
		for (size_t b_=0; b_ < data->bsize; b_++) {
			for (size_t i=0; i < ses->inputs + ses->outputs; i++) {
				freets(data->batch[b]->line[b_]->ts[i]);
			}
			free(data->batch[b]->line[b_]->ts);
			free(data->batch[b]->line[b_]);
		}
		free(data->batch[b]->line);
		free(data->batch[b]);
	}
	free(data->batch);
	free(data);
};