from .models.model import *
from .Objects import Model
from .models import fllc
from .models.join_neurones import join_neurones

class FLLC:	#fully connected
	def FC1D(params:str, session, programs):	#full conn 1d
		'''
Usage:
	struct -params
struct:
	:int str ; int str ; int str ...
	int - nb neurones in this layer
	str - name of the function ('str')
params:
	-rnd	randomise all weights and bias (default:False)
	-drop_out:float
exemple:
	Gen.FLLC.FC1D 2 'identity'; 3 'Tanh'; 1 'Gauss' -rnd
		'''
		rnd, drop_out = False, False
		if '-rnd' in params:
			rnd = True
			params = params.replace('-rnd', '').strip()

		if '-drop_out:' in params:
			drop_out = True
			params = params.replace('-rnd', '').strip()
	
		struct_ = [x.strip().split(' ') for x in params.split(';')]
		struct_ = [(int(i),s.replace("'", '')) for i,s in struct_]

		return Model(fllc.fc1d(*struct_, rnd=rnd, drop_out=drop_out))

	def FULLCND(params:str, session, programs):
		'''
Usage:
	
exemple:
	Gen.FLLC.FC 2:'' 3:'Gauss' '1':Gauss -rnd -rnn
		'''
		print('\033[96mDROP_OUT=0, RNN=False\033[0m')
		return Model(fllc.fullcnd(((2,),''), ((3,),'Gauss'), ((1,),'Gauss'), rnd=True, drop_out=0, rnn=True))

class NEURONES:
	def neurones(params:str, session, programs):
		'''
exemple:
	NEURONES.neurones neuronesfile
file:
0	#neurone 0
mats...
code...
1	#neurone 1
...
end	#end of neurones declaring, bebin of connections
0.0 -> 7.1	#0'th output of 0'th neurone goes to 1'st input of 7'th neurone  (translate by : $7.1 = identity($0.0))
0.1 -> 12.0	#1'th output of 0'th neurone goes to 0'st input of 12'th neurone
		'''
		with open(params, 'r') as co:	#or 'wb'
			content = co.read()

		return join_neurones(content)
