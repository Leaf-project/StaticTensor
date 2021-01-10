double mbe(double want, double get) {
	return (want - get);
};

double mse(double want, double get) {
	return pow(want - get, 2)/2;
};

double mae(double want, double get) {
	return fabs(want - get)/2;
};

double cel(double want, double get) {
	return -(want*log10(get) + (1-want)*log10(1 - get));
};