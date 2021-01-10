from ..io import get_free_tmp_file, readmdl
import time, subprocess, os
from .Objects import Model

def simple(params, session, programs):
	''' ?-unmissable - missable
Usage:
	Gen.full_conn_1d mdl:? data:? mdt:- echopes:? loss:- score:-
exemple:
	Train.simple mdl:xor data:data mdt:'RMSProp' echopes:100
	'''
	ptr = False
	if '-ptr' in params:
		ptr = True
		params = params.replace('-ptr', '').strip()

	dico = {x.split(':')[0]:x.split(':')[1] for x in params.split(' ')}

	for name in ('loss', 'mdt', 'score'):
		if not name in dico: dico[name] = 'default'
	
	dico['data'] = session.vars[dico['data']]
	dico['mdl'] = session.vars[dico['mdl']]
	dico['mdt'] = dico['mdt'].strip("'")
	dico['loss'] = dico['loss'].strip("'")
	dico['score'] = dico['score'].strip("'")

	sesfile = get_free_tmp_file()		#sesfile
	datafile = get_free_tmp_file()		#datafile
	sesfilesave = get_free_tmp_file()	#sesfilesave

	dico['data'].save(datafile, session, programs)
	dico['mdl'].save(sesfile, session, programs)

	start = time.time()
	if ptr:
		print('Executing:',programs['normal_train'] + ' ' + sesfile + ' ' + datafile + ' ' + sesfilesave + ' ' + dico['echopes'] + ' ' + dico['mdt'] + ' ' + dico['loss'] + ' ' + dico['score'] + (' -ptr' if ptr else ''))
	train = subprocess.Popen(programs['normal_train'] + ' ' + sesfile + ' ' + datafile + ' ' + sesfilesave + ' ' + dico['echopes'] + ' ' + dico['mdt'] + ' ' + dico['loss'] + ' ' + dico['score'] + (' -ptr' if ptr else ''), shell=True)
	train.wait()
	train.kill()
	print(f'Trainning take {str(time.time()-start)} seconds')

	model_ret = Model(readmdl(sesfilesave))

	for file in (sesfile, datafile, sesfilesave):
		os.system('rm ' + file)

	return model_ret