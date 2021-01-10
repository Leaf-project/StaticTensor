Tensor*** _read_tsss(size_t times, size_t vars, FILE *fp) {
	Tensor** *ret = (Tensor***)malloc(times * sizeof(Tensor**));
	for (size_t t=0; t < times; t++) {
		ret[t] = (Tensor**)malloc(vars * sizeof(Tensor*));
		for (size_t i=0; i < vars; i++) {
			ret[t][i] = readts(fp);
		}
	}
	return ret;
};

void _write_tsss(Tensor** *tsss, size_t times, size_t vars, FILE *fp) {
	for (size_t t=0; t < times; t++) {
		for (size_t i=0; i < vars; i++) {
			writets(tsss[t][i], fp);
		}
	}
};

Session* load_ses(char *file) {
	FILE *fp = fopen(file, "rb");
	//
	Session *ret = (Session*)malloc(sizeof(Session));
	//
	fread(&ret->vars, sizeof(size_t), 1, fp);
	fread(&ret->inputs, sizeof(size_t), 1, fp);
	fread(&ret->outputs, sizeof(size_t), 1, fp);
	//
	ret->input = (size_t*)malloc(ret->inputs * sizeof(size_t));
	ret->output = (size_t*)malloc(ret->outputs * sizeof(size_t));
	//
	fread(ret->input, sizeof(size_t), ret->inputs, fp);
	fread(ret->output, sizeof(size_t), ret->outputs, fp);
	//
	fread(&ret->times, sizeof(size_t), 1, fp);
	//
	ret->var = _read_tsss(ret->times, ret->vars, fp);
	ret->grad = _read_tsss(ret->times, ret->vars, fp);
	//
	fread(&ret->codelen, sizeof(size_t), 1, fp);
	ret->code = malloc(ret->codelen + 1);
	fread(ret->code, sizeof(char), ret->codelen, fp);
	ret->code[ret->codelen] = '\0';
	//
	fclose(fp);
	//Security checks
	track_unused(ret);
	//
	return ret;
};

Session* cpyses(Session *ses) {
	Session *ret = (Session*)malloc(sizeof(Session));
	//
	memcpy(ret, ses, sizeof(Session));
	//
	ret->input = (size_t*)malloc(ret->inputs * sizeof(size_t));
	ret->output = (size_t*)malloc(ret->outputs * sizeof(size_t));
	//
	memcpy(ret->input, ses->input, ret->inputs * sizeof(size_t));
	memcpy(ret->output, ses->output, ret->outputs * sizeof(size_t));
	//
	ret->var = (Tensor***)malloc(ret->times * sizeof(Tensor**));
	ret->grad = (Tensor***)malloc(ret->times * sizeof(Tensor**));
	//
	for (size_t t=0; t < ret->times; t++) {
		ret->var[t] = (Tensor**)malloc(ret->vars * sizeof(Tensor*));
		ret->grad[t] = (Tensor**)malloc(ret->vars * sizeof(Tensor*));
		for (size_t i=0; i < ret->vars; i++) {
			ret->var[t][i] = cpyts(ses->var[t][i]);
			ret->grad[t][i] = cpyts(ses->grad[t][i]);
		}
	}
	//
	ret->code = malloc(ses->codelen + 1);
	memcpy(ret->code, ses->code, ret->codelen+1);
	//
	return ret;
};

void freeses(Session *ses) {
	//Free Vars
	for (size_t t=0; t < ses->times; t++) {
		for (size_t i=0; i < ses->vars; i++) {
			freets(ses->var[t][i]);
		}
		free(ses->var[t]);
	}
	free(ses->var);
	//Free Grads
	for (size_t t=0; t < ses->times; t++) {
		for (size_t i=0; i < ses->vars; i++) {
			freets(ses->grad[t][i]);
		}
		free(ses->grad[t]);
	}
	free(ses->grad);
	//
	free(ses->input);
	free(ses->output);
	//
	free(ses);
};

void save_ses(Session *ses, char *file) {
	FILE *fp = fopen(file, "wb");
	//
	fwrite(&ses->vars, sizeof(size_t), 1, fp);
	fwrite(&ses->inputs, sizeof(size_t), 1, fp);
	fwrite(&ses->outputs, sizeof(size_t), 1, fp);
	//
	fwrite(ses->input, sizeof(size_t), ses->inputs, fp);
	fwrite(ses->output, sizeof(size_t), ses->outputs, fp);
	//
	fwrite(&ses->times, sizeof(size_t), 1, fp);
	//
	_write_tsss(ses->var, ses->times, ses->vars, fp);
	_write_tsss(ses->grad, ses->times, ses->vars, fp);
	//
	fwrite(&ses->codelen, sizeof(size_t), 1, fp);
	fwrite(ses->code, sizeof(char), ses->codelen, fp);
	//
	fclose(fp);
};