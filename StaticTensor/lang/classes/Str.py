def cvs(params:str, session, programs):
	'''
Usage:
	Str.cvs str
	str: 'line0\nline1\n...lineN'
Exemple:
	Str.cvs '0,0;0\n0,1;1\n1,0;1\n1,1;0'
	'''
	return params.replace("'", '').split('\\n')

def loadcvs(params:str, session, programs):
	'''
Usage:	return str
	Str.loadcvs file
Exemple:
	Str.loadcvs '/media/henry/USB_DISK/Informatics/btc_data/btc.cvs'
	'''
	with open(params.replace("'", ''), 'r') as co:
		return co.read().split('\n')