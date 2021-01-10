#pragma once

#include "tensor.h"

typedef Tensor* (*Opti)(Tensor*,Tensor*,double**);

typedef struct {
	Tensor** *var;	//[time][id] -> Tensor*	| first time never goes out, it's the reference, allways set to 0
	Tensor** *grad;	//[time][id] -> Tensor*	| where is the sum of all grads
	//
	char* code;
	size_t codelen;
	//
	size_t vars;	//how many vars
	size_t inputs;	//how many inputs
	size_t outputs;	//how many outputs
	size_t *input;	//input[id]	-> pos
	size_t *output;	//output[id] -> pos
	//
	size_t times;	//how many times
} Session;

//Memory
void track_unused(Session *ses);
Session* load_ses(char *file);
Session* cpyses(Session *ses);
void freeses(Session *ses);
void save_ses(Session *ses, char *file);
#include "../src/session/mem.c"

//Security Mesure
////void track_unused(Session *ses);
#include "../src/session/security.c"

//Controle
void rnd_noise_cells(Session *ses, size_t time);
void restart(Session *ses, size_t time);
void clear(Session *ses);	//clear from 1 to ses->times (not the first)
void expand(Session *ses);
void sum_grads(Session* ses);
#include "../src/session/controle.c"

//Str
void ptrgrad(Session *ses);
void ptrvar(Session *ses);
void ptrcode(Session *ses);
void ptrses(Session *ses);
#include "../src/session/str.c"

//Compute
void forward(Session *ses, size_t time);
void backward(Session *ses, size_t time);
#include "../src/session/compute.c"

//Update
void update(Session *ses, Opti opti, double** *pass);	//update from grad[grad_time] (should be 0) to var[var_t]
#include "../src/session/update.c"