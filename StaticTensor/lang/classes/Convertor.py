import struct
from .models.model import mkts, mul
from .Objects import Data

def _cvs_to_bin(lines, bsize, inputs, outputs):
	data = [map(float, line.replace(';',',').split(',')) for line in lines]
	bin_data = struct.pack('NN', bsize, len(lines)//bsize)
	for line in data:	#where line is iter(0.0,12.0,-5.1,-0.002)
		for shape in inputs+outputs:
			bin_data += mkts(shape, [next(line) for x in range(mul(shape))])
	return bin_data

def _cvs_to_brut(filefrm, fileto, bsize, inputs, outputs):
	#Where inputs/outputs:[shape, shape, ...], shape:{int,int...}
	
	with open(filefrm, 'r') as co:
		#map gives iter object compatible with next
		co = co.read().replace('\n\n', '\n').split('\n')
		if '' in co: del co[co.index('')]
		lines = len(co)
		data = [map(float, line.replace(';',',').split(',')) for line in co]

	with open(fileto, 'wb') as co:
		co.write(struct.pack('NN', bsize, lines//bsize))
		for line in data:	#where line is iter(0.0,12.0,-5.1,-0.002)
			for shape in inputs+outputs:
				co.write(mkts(shape, [next(line) for x in range(mul(shape))]))

def cvs_to_bin(params:str, session, programs):
	''' '?'=obligatoire
Usage:
	Convertor.cvs_to_bin cvs:? bsize:? inputs:? outputs:?
	bsize - batch size (int)
	*inputs
exemple:
	Convertor.cvs_to_bin mdl:xor cvs:cvs
	'''
	dico = {x.split(':')[0]:x.split(':')[1] for x in params.split(' ')}

	dico['lines'] = session.vars[dico['cvs']]
	dico['bsize'] = int(dico['bsize'])
	for name in ('inputs', 'outputs'):
		dico[name] = list(map(lambda x:list(map(int,x.split(','))), dico[name].strip(';').split(';')))
	del dico['cvs']

	return Data(_cvs_to_bin(**dico))