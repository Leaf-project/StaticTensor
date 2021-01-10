#pragma once

#include <stdlib.h>
#include <string.h>

#define MIN_RND -1
#define MAX_RND 1

double rand_double() {
	double scale = rand() / (double) RAND_MAX; /* [0, 1.0] */
	return MIN_RND + scale * ( MAX_RND - MIN_RND );      /* [min, max] */
}

size_t mul(size_t *lst, size_t len) {
	size_t ret=1;
	for (size_t i=0; i < len; i++) ret *= lst[i];
	return ret;
};

double sign(double x) {
	if (x >= 0) return 1.0;
	else return -1.0;
}

char *dtoa(double a, const char* s) {	//double to string
	size_t len = snprintf(NULL, 0, s, a);
	char* str = malloc( len + 1 );
	snprintf(str, len+1, s, a);
	return str;
};

char *sttoa(size_t a, const char* s) {	//size_t to string
	size_t len = snprintf(NULL, 0, s, a);
	char* str = malloc(len + 1);
	snprintf(str, len+1, s, a);
	return str;
};

void freestrs(char* *strs) {
	char *str; size_t lp=0;
	while ((str = strs[lp++])[0] != '\0') free(str);
	free(strs);
};

void ptrstrs(char **strs) {
	char *str; size_t lp=0;
	while ((str = strs[lp++])[0] != '\0') {
		printf("%s|\n", str);
	};	
};

char *newline(size_t len) {
	char *ret = malloc(len);
	memset(ret, ' ', len-1);
	ret[len-1]='\0';
	return ret;
};

size_t lineslen(char* *strss) {
	char *str;size_t lines = 0;
	while ((str = strss[lines++])[0] != '\0'){};
	return lines-1;
};