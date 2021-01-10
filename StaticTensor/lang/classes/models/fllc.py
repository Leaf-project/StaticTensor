from .model import *

#struct exemple : ((2, identity), (3, Tanh), (1, Logistic))
def fc1d(*struct, rnd=True, drop_out=False):	#fully connected 1d
	#1d meen [1,1,1,1..,1] but 2d shape (for weights)
	#lay0 = f(lay0 + bias0)
	model = {
		'code' : [
			*cmd(2,0, 0,0, 5,0, '+'), 
			*cmd(3,0, 2,0, funcs.index(struct[0][1]), ')')
		],
		'mats' : [
			[[struct[0][0], 1], [0.0 for _ in range(struct[0][0])], 0],	#lay input (like previous activation (but there is not previous layer))
			[[1,1], [1.0], 0],	#lay0 @w
			[[struct[0][0], 1], [0.0 for _ in range(struct[0][0])], 0],	#lay0 +b
			[[struct[0][0], 1], [0.0 for _ in range(struct[0][0])], 0],	#lay0 f()
			[[1,1], [1.0], 1],											#Techniquelly weigth but not
			[[struct[0][0], 1], [(1.0 if not rnd else random.uniform(-1, 1)) for _ in range(struct[0][0])], 1]	#bias
		],
		'inputs' : [0],
		'outputs' : [None]	#Will be set at end
	}
	#
	for lay in range(1, len(struct)):
		c = len(model['mats'])
		model['mats'] += [
			[[struct[lay][0],1], [0.0 for _ in range(struct[lay][0])], 0],#layer @w
			[[struct[lay][0],1], [0.0 for _ in range(struct[lay][0])], 0],#layer +b
			[[struct[lay][0],1], [0.0 for _ in range(struct[lay][0])], 0],#Add f()

			[[struct[lay][0],struct[lay-1][0]], [(1.0 if not rnd else random.uniform(-1, 1)) for _ in range(struct[lay-1][0]*struct[lay][0])], 1],	#Weigth
			[[struct[lay][0],1], [(1.0 if not rnd else random.uniform(-1, 1)) for _ in range(struct[lay][0])], 1] #Bias
		]
		#code
		model['code'] += cmd(c,0,	c-3,0,	c+3,0,	'@')	# lay = lay[-1] @ weigths
		model['code'] += cmd(c+1,0,	c,0,	c+4,0,	'+')	# lay = lay + bias
		model['code'] += cmd(c+2,0,	c+1,0, funcs.index(struct[lay][1]), ')')	# lay = f(lay)

	model['outputs'][0] = len(model['mats'])-3
	
	return model

arr	= lambda len,rnd=False: [(0.0 if not rnd else random.uniform(-1, 1)) for _ in range(len)]

#def arr(len, rnd=False):
#	return [(0.0 if not rnd else random.uniform(-1, 1)) for _ in range(len)]

arrx = lambda len,x: [x for _ in range(len)]

#struct : ((2,),'identity'), ((3,), 'Gauss'), ((1,), 'Gauss')
#Complexite pythonienne 
def fullcnd(*struct, rnd=True, drop_out=0, rnn=False):	#fc:'fully connected' (tensors)
	if drop_out > 1: exit()
	isdrop = int(bool(drop_out))
	model = {
		'code' : [],
		'mats' : [
			[[*struct[0][0],1], arr(mul(struct[0][0])), 0], #$0 input
			[[*struct[0][0],1], arr(mul(struct[0][0])), 0], #$1=$0+$bias
			[[*struct[0][0],1], arr(mul(struct[0][0]),rnd=rnd), 1]	#$bias
		],
		'inputs' : [0],
		'outputs' : [None]	#Will be set at end
	}
	#Adding bias to input
	model['code'] += cmd(1,0, 0,0, 2,0, '+')
	#
	pre_n = struct[0][0]
	first_lay = True
	for shp,f in struct[1:]:
		c = len(model['mats'])	#old mats lenght
		#
		model['mats'] += [
			*[[[*shp, 1], arr(mul(shp)), 0] for _ in range(3 + (0,3)[isdrop] + (0,2)[rnn])],#($0,$1 dropout),$2,($3,$4 rnn),$5,$6,($7 dropout)
			[[shp[0],pre_n[0],*shp[2:]], arr(mul([*shp[0:1],pre_n[0],*shp[2:]]),rnd), 1],	#$8	 weight $w0	prev_lay,this lay
		]
		if rnn:
			model['mats'] += [[[shp[0],shp[0],*shp[2:]], arr(mul([*shp,shp[0]]),rnd), 1]]	#$9	 weight $w1 this,this
		
		model['mats'] += [[[*shp, 1], arr(mul(shp),rnd), 1]]	#$10 adding bias
		if drop_out:
			model['mats'] += [
				[[*shp, 1], arr(mul(shp)), 4],					#$11	$noise	
				[[*shp, 1], arrx(mul(shp),2*drop_out-1), 2],	#$12	(2*$p_drop_out-1) scale rnd(-1,1) to a (0,1) probabilities (const)
			]
		#CODE
		if drop_out:
			model['code'] += [
				*cmd(c+0,0, c+11,0, c+12,0, '+'),					#$0 = $noise + (2*$p_drop_out-1) $0 will be (0;1)
				*cmd(c+1,0, c+0,0, funcs.index('bin_step'), ')'),	#$1 = bin($0)
			]
		model['code'] += cmd(
			c+(0,2)[isdrop],0, 
			(1 if first_lay else c-3-(0,2)[isdrop]-(0,1)[rnn]),0,
			c+3+(0,2)[rnn]+(0,3)[isdrop],0,
			'@')
		if rnn:
			model['code'] += [
				*cmd(c+1+(0,2)[isdrop],0, c+4+(0,3)[isdrop],1, c+6+(0,3)[isdrop],0, '@'),	#($3 = $out[-1] @ $w1) if rnn	history
				*cmd(c+2+(0,2)[isdrop],0, c+(0,2)[isdrop],0, c+1+(0,2)[isdrop],0, '+'),	#($4 = $2 + $3) if rnn
			]
		model['code'] += cmd(#bias
			c+1+(0,2)[rnn]+(0,2)[isdrop],0,
			c+0+(0,2)[rnn]+(0,2)[isdrop],0,
			c+4+(0,3)[rnn]+(0,4)[isdrop],0, '+')
		model['code'] += cmd(#activation
			c+2+(0,2)[rnn]+(0,2)[isdrop],0,
			c+1+(0,2)[rnn]+(0,2)[isdrop],0,
			funcs.index(f), ')')
		if drop_out:#last drop out
			cmd(c+5+(0,2)[rnn],0, c+4+(0,2)[rnn],0, c+1,0, '*')
		#	#$0 = $noise + (2*$p_drop_out-1) $0 will be (0;1)
		#	#$1 = bin($0)
		#$2 = $l-1 @ $w0		l-1 is last layer
		#	#($3 = $out[-1] @ $w1) if rnn	history
		#	#($4 = $2 + $3) if rnn
		#$5 = $4 + $b #adding bias
		#$6 = f($5)	(output for the network)
		#	#$7 = $6 * $1	<- drop out (output for next layer)
		#$8		w normal
		#	#$9		w rnn
		#$10	bias
		#	#$11	noise
		#	#$12	recify noise

		first_lay = False
		pre_n = shp

	model['outputs'][0] = len(model['mats'])-3-int(rnn)-(0,2)[isdrop]
	
	return model