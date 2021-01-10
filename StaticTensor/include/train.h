#pragma once

#include "data.h"

Tensor* Sgd(Tensor *w, Tensor *dw, double **pass);
Tensor* Momentum(Tensor *w, Tensor *dw, double **pass);
Tensor* RMSProp(Tensor *w, Tensor *dw, double **pass);
Tensor* AdaGrad(Tensor *w, Tensor *dw, double **pass);
Tensor* Adam(Tensor* w, Tensor* dw, double **pass);
//
double*** mkpass(Session *ses);
void freepass(double** *pass, Session *Ses);
#include "../src/train/opti.c"

//Train
void train_batch(Session* ses, Batch* batch, size_t bsize, Opti opti, Loss loss, Scorefn scorefn, double** *past_pass);
void train(Session* ses, Data* data, Opti opti, Loss loss, Scorefn scorefn, size_t echopes, char ptr);	//Normal trainning passing all batchs
void train_only_worst(Session* ses, Data* data, Opti opti, Loss loss, Scorefn scorefn, size_t echopes);
#include "../src/train/train.c"