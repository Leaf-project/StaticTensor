/*
In the actual mdl[time] all vars doesn't existe. All are free pointers.
Each vars change only one time. We can directly replace pointer by the 
func : vars_id:23, time:0
ses->mdl->mats[0][23] = func();
where func() -> Tensor*
*/

#define Y 	ses->var[time-arg[1]][arg[0]]
#define dY 	ses->grad[time-arg[1]][arg[0]]
#define A 	ses->var[time-arg[3]][arg[2]]
#define dA 	ses->grad[time-arg[3]][arg[2]]
#define B 	ses->var[time-arg[5]][arg[4]]
#define dB 	ses->grad[time-arg[5]][arg[4]]

void forward(Session *ses, size_t time) {
	rnd_noise_cells(ses, time);
	//
	if (time == 0) {
		printf("\033[95mCan't forward time 0.\033[0m\n");
		exit(1);
	}
	//
	size_t arg[6] = {0,0,0,0,0,0};	//six is juste max amount of arguments in one command
	size_t arg_pos = 0;
	//
	char chr;size_t pos=0;	//pos in code
	while (pos < ses->codelen) {
		chr = ses->code[pos++];
		//printf("For:%li chr:%i\n", pos, chr);
		if (chr < 10) arg[arg_pos] = arg[arg_pos]*10 + chr;
		else if (chr == 10) arg_pos++; //	, comma separate args
		else if (11 <= chr && chr <= 15)	Y = operator(A, B, ops[chr-11]);
		else if (chr == 16) 				Y = dot(A, B);
		else if (17 <= chr && chr <= 21)	Y = iter(A,	ops[chr-17]);
		else if (chr == 22) 				Y = transpose(A);
		else if (chr == 23) 				Y = (*funcs[arg[4]])(A);
		//
		if (chr > 10) {
			//ptrts(Y);
			for (size_t i=0; i < 6; i++) arg[i]=0;
			arg_pos = 0;
		}
	};
};

//quand on backward pour : ((trgt,tm),(x,tm)), on a {0,0,trgt,tm,x,tm}, decalage de 2
void _decale_args(size_t args[], size_t nb) {
	for (size_t i=nb; i < 6; i++) args[i-nb] = args[i];
};

#define ln(x) log(x) 
//in C log is ln in math, and log10 in C is log in math

void backward(Session *ses, size_t time) {
	Tensor *tmp0, *tmp1;
	double tmp_double0, tmp_double1;
	//
	size_t arg[6] = {0,0,0,0,0,0};	//six is juste max amount of arguments in one command
	size_t arg_pos = 5;
	size_t pow10level=0;
	//
	char chr;size_t pos=ses->codelen-1;	//pos in code
	char last_op=ses->code[ses->codelen-1];
	//
	while (pos-- != 0) {
		chr = ses->code[pos];
		//printf("Backprop Chr:%i pos:%li last_op:%i\n", chr, pos, last_op);
		if (chr < 10) {
			arg[arg_pos] += chr * pow(10, pow10level++);
		} else if (chr == 10) {
			arg_pos--;
			pow10level=0;
		}
		if (chr > 10) {
			if (last_op == 11 || last_op == 12) {	//	+ -
				for (size_t i=0; i < A->len; i++) {
					dA->arr[i] += dY->arr[i];
					dB->arr[i] += dY->arr[i];
				}
			} else if (last_op == 13) {	//	*
				for (size_t i=0; i < dA->len; i++) {
					dA->arr[i] += (dY->arr[i] * B->arr[i]);
					dB->arr[i] += (dY->arr[i] * A->arr[i]);
				}
			} else if (last_op == 14) {	//	/  always adding derivates
				//y = a/b where (da = dy/b) and (db = dy*a*-1/(b**2 + 1e-10)) +1e-10 to avoid ZeroDivizionError because b**2 will be >= 0
				for (size_t i=0; i < A->len; i++) {
					dA->arr[i] += (dY->arr[i]/B->arr[i]);
					dB->arr[i] -= dY->arr[i] * A->arr[i] / (pow(B->arr[i], 2)+1e-10);
				}
			} else if (last_op == 15) {	//	^ //da,b,c;		da,a,b
				//y = a^b where (da = dy*b*a^(b-1)) and (db = dy*y*ln(a))
				for (size_t i=0; i < Y->arr[i]; i++) {
					dA->arr[i] += dY->arr[i]*B->arr[i]*pow(A->arr[i], B->arr[i]-1);
					dB->arr[i] += dY->arr[i]*Y->arr[i]*ln(A->arr[i]);
				}
				//
			} else if (last_op == 16) {	//	@
				//y = a @ b where (da = dy @ b.T) and (db = a.T @ dy)
				//da
				tmp0 = transpose(B);	//b.T
				tmp1 = dot(dY, tmp0);	//dy @ b.T
				for (size_t i=0; i < A->len; i++)
					dA->arr[i] += tmp1->arr[i];
				freets(tmp0);
				freets(tmp1);
				//db
				tmp0 = transpose(A);	//a.T
				tmp1 = dot(tmp0, dY);	//a.T @ dy
				for (size_t i=0; i < B->len; i++)
					dB->arr[i] += tmp1->arr[i];
				freets(tmp0);
				freets(tmp1);
			} else if (17 <= last_op && last_op <= 21) {	//iter + - * / ^
				_decale_args(arg, 2);	//TODO
				//y = iter(a, op)
				if (last_op == 17 || last_op == 18 || last_op == 19) {
					//da = dy*a/y (where y->len == 1, sum or sub or mul of array is always 1)
					for (size_t i=0; i < A->len; i++) {
						dA->arr[i] = dY->arr[0]*A->arr[i]/Y->arr[0];
					}
				} else if (last_op == 20) {	//div
					//y = x0 / (x1 / (x2/ x3 ...))
					tmp_double0 = Y->arr[0];
					tmp_double1 = dY->arr[0];
					for (int i=A->len-1; i >= 0; i--) {
						tmp_double0 *= A->arr[i];
						dA->arr[i] = -tmp_double0*tmp_double1/pow(A->arr[i], 2);
						tmp_double1 /= A->arr[i];
					}
				} else if (last_op == 21) { //pow
					tmp_double0 = Y->arr[0];
					tmp_double1 = 1.0;
					// y = a**b
					for (int i=A->len-1; i >= 0; i--) {
						dA->arr[i] = -tmp_double1*tmp_double0*log(pow(tmp_double0, 1/A->arr[i]));	//e*y*log(y**(1/b))
						tmp_double0 = pow(tmp_double0, 1/A->arr[i]);
						tmp_double1 *= A->arr[i] * pow(tmp_double0, A->arr[i]-1);
					}
				}
			} else if (last_op == 22) { //	T
				_decale_args(arg, 2);
				freets(dA);
				dA = transpose(dY);
			} else if (last_op == 23) {	//	) dfunc
				_decale_args(arg, 1);
				//
				tmp0 = (*dfuncs[arg[4]])(A);	//because can be Softmax or maxout
				//
				for (size_t i=0; i < dA->len; i++) {
					dA->arr[i] += tmp0->arr[i] * dY->arr[i];
				}
				//
				freets(tmp0);
			}
			last_op = chr;
			for (size_t i=0; i < 6; i++) arg[i]=0;
			arg_pos = 5; pow10level = 0;
		}
	}
};