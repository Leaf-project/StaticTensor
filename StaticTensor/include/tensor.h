#pragma once

#include "debbug.h"
#include "consts.h"

#include <stdlib.h>
#include <string.h>

//Array
typedef struct {
	size_t deep;
	size_t *shape;
	//
	double *arr;
	//
	size_t *real_sizes;
	size_t len;
	char type;	//0-var, 1-weigth, 2-const, 3-unused, 4-noise(random(-1,1))
} Tensor;		//sizeof(Tensor) == 48

//Memory
Tensor* mkts(size_t deep, size_t *shape, double *arr, char type);
Tensor* cpyts(Tensor *ts);
void set_eq_ts(Tensor *dest, Tensor *src);
void freets(Tensor *ts);
#include "../src/tensor/mem.c"

//io operations
void writets(Tensor *ts, FILE* fp);
Tensor* readts(FILE *fp);
#include "../src/tensor/io.c"

//Controle
size_t realpos(Tensor *ts, size_t *indexs);
void filltslist(Tensor *ts, double *value);
void fillts(Tensor *ts, double value);
void noisets(Tensor *ts);
#include "../src/tensor/controle.c"

//Str
char** strts(Tensor *ts);
void ptrts(Tensor *ts);
void ptrtsshape(Tensor *ts);
#include "../src/tensor/str.c"

//Operations
Tensor* operator(Tensor *a, Tensor *b, Op op);
Tensor* iter(Tensor *ts, Op op);
Tensor* activate(Tensor *ts, Fn fn);
Tensor* transpose(Tensor *ts);
Tensor* dot(Tensor *a, Tensor *b);
#include "../src/tensor/op.c"

//Debbug
void debbug_ts(Tensor* ts);
#include "../src/tensor/debbug.c"

#include "funcs.h"	//<-- here is defined all funcs
