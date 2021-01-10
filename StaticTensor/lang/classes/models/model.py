import struct
import random

funcs = [
	'identity', 'xor_fn', 'bin_step', 'Logistic', 
	'Tanh', 'ReLU', 'GELU', 
	'SoftPlus', 'ELU', 'SELU', 
	'Leaky_ReLU', 'Softsign', 'SQNL',
	'Bent_Ind', 'Gauss', 
	'SQ_RBF', 'SoftMax', 'Maxout'
]

cmds = {
	',':10,
	'+':11, '-':12, '*':13, '/':14, '^':15,
	'@':16,
	'iter +':17, 'iter -':18, 'iter *':19, 'iter /':20, 'iter ^':21,
	'T':22,
	')':23
}

int_to_chr = lambda x: list(map(lambda j:int(j), str(x)))
cmd = lambda *params: [x for p in params[:-1] for x in int_to_chr(p)+[cmds[',']]][:-1] + [cmds[params[-1]]]

def mul(lst):
	a = 1
	for i in lst: a *= i
	return a

mkts = lambda shape, arr: struct.pack('N'*(1 + len(shape))+'d'*len(arr)+'B', len(shape), *shape, *arr, 0)
