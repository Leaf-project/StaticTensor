#pragma once

#include <math.h>

typedef Tensor* (*Activation)(Tensor*);

//Repited Patern
Tensor* func(Tensor* x, Fn fn);
//Classic
Tensor* identity(Tensor *x);
Tensor* xor_fn(Tensor *x);
Tensor* bin_step(Tensor *x);
Tensor* Logistic(Tensor *x);
Tensor* Tanh(Tensor *x);
Tensor* ReLU(Tensor *x);
Tensor* GELU(Tensor *x);
Tensor* SoftPlus(Tensor *x);
Tensor* ELU(Tensor *x);
Tensor* SELU(Tensor *x);
Tensor* Leaky_ReLU(Tensor *x);
Tensor* Softsign(Tensor *x);
Tensor* SQNL(Tensor *x);
Tensor* Bent_Ind(Tensor *x);
Tensor* Gauss(Tensor *x);
Tensor* SQ_RBF(Tensor *x);
//Spetials
Tensor* SoftMax(Tensor *x);
Tensor* Maxout(Tensor *x);
#include "../src/funcs/funcs.c"

Activation funcs[18] = {identity, xor_fn, bin_step, Logistic, Tanh, ReLU, GELU, SoftPlus, ELU, SELU, Leaky_ReLU, Softsign, SQNL, Bent_Ind, Gauss, SQ_RBF, SoftMax, Maxout};

//Derivatives

Tensor* didentity(Tensor* x);
Tensor* dxor_fn(Tensor *x);
Tensor* dbin_step(Tensor* x);
Tensor* dLogistic(Tensor* x);
Tensor* dTanh(Tensor* x);
Tensor* dReLU(Tensor* x);
Tensor* dGELU(Tensor* x);
Tensor* dSoftPlus(Tensor* x);
Tensor* dELU(Tensor* x);
Tensor* dSELU(Tensor* x);
Tensor* dLeaky_ReLU(Tensor* x);
Tensor* dSoftsign(Tensor* x);
Tensor* dSQNL(Tensor* x);
Tensor* dBent_Ind(Tensor* x);
Tensor* dGauss(Tensor* x);
Tensor* dSQ_RBF(Tensor* x);
//
Tensor* dSoftMax(Tensor* x);
Tensor* dMaxout(Tensor* x);
#include "../src/funcs/dfuncs.c"

Activation dfuncs[18] = {didentity, dxor_fn, dbin_step, dLogistic, dTanh, dReLU, dGELU, dSoftPlus, dELU, dSELU, dLeaky_ReLU, dSoftsign, dSQNL, dBent_Ind, dGauss, dSQ_RBF, dSoftMax, dMaxout};
