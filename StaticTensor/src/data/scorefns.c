double sum_score(double* list, size_t len) {
	double ret = 0.0;
	for (size_t i=0; i < len; i++)
		ret += fabs(list[i]);
	return ret;
};

double moy_score(double* list, size_t len) {
	double ret = 0.0;
	for (size_t i=0; i < len; i++) ret += list[i];
	return ret / len;
};