from ..io import *

import subprocess

class Model:
	def __init__(self, model):
		self.model = model

	def show_result(self, params:str, session, programs):
		v_by_v = 0

		if '-v_by_v' in params:
			params = params.replace('-v_by_v', '')
			params = params.strip(' ')
			v_by_v = 1
		
		dico = {x.split(':')[0]:x.split(':')[1] for x in params.strip().split(' ')}

		for name in ('loss', 'score'):
			if not name in dico:
				dico[name] = 'default'

		dico['data'] = session.vars[dico['data']]
		dico['loss'] = dico['loss'].strip("'")
		dico['score'] = dico['score'].strip("'")

		sesfile = get_free_tmp_file()		#sesfile
		datafile = get_free_tmp_file()		#datafile

		dico['data'].save(datafile, session, programs)
		self.save(sesfile, session, programs)

		print('Executing:', programs['show_results'] + ' ' + sesfile + ' ' + datafile + ' ' + dico['loss'] + ' ' + dico['score'] + (' -v_by_v' if v_by_v else ''))
		train = subprocess.Popen(programs['show_results'] + ' ' + sesfile + ' ' + datafile + ' ' + dico['loss'] + ' ' + dico['score'] + (' -v_by_v' if v_by_v else ''), shell=True)
		train.wait()
		train.kill()

		for file in (sesfile, datafile):
			os.system('rm ' + file)

	def save(self, params:str, session, programs):
		writemdl(self.model, params.strip("'"))

	@staticmethod
	def load(params:str, session, programs):
		return Model(readmdl(params.strip("'")))

	def ptr(self, params:str, session, programs):
		if params == 'normal': pass
		#
		sesfile = get_free_tmp_file()
		self.save(sesfile, session, programs)
		#
		train = subprocess.Popen(programs['ptrses'] + ' ' + sesfile, shell=True)
		train.wait()
		train.kill()
		#
		os.system('rm ' + sesfile)

class Data:
	def __init__(self, data):
		self.data = data #this is a bytes object (ok to write)

	def save(self, params:str, session, programs):
		writedata(self.data, params.strip("'"))

	@staticmethod
	def load(params:str, session, programs):
		Data(readdata(params.strip("'")))

	def ptr(self, params:str, session, programs):
		if params == 'normal': pass
		#
		sesfile = get_free_tmp_file()
		datafile = get_free_tmp_file()
		dico['data'].save(datafile, session, programs)
		self.save(sesfile, session, programs)
		#
		train = subprocess.Popen(programs['ptrdata'] + ' ' + sesfile + ' ' + datafile, shell=True)
		train.wait()
		train.kill()
		#
		for file in (sesfile, datafile):
			os.system('rm ' + file)