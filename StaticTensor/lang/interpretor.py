from .io import writemdl, readmdl, get_free_tmp_file
from .classes.Objects import Model, Data
from .classes import Str, Convertor, Train, Gen
import os, copy, subprocess, time, struct, random

random.seed(1)

programs = {
	'normal_train'	:	'./bin/normal_train.exe',
	'ptrdata'		: 	'./bin/ptrdata.exe',
	'ptrses'		:	'./bin/ptrses.exe',
	'show_results'	:	'./bin/show_results.exe'
}

class Session:
	vars = {}

	@staticmethod
	def delcoms(str):
		if (pos:=str.find('#')) != -1:
			str = str[:pos]
		return str.strip()

	def new(self, line):
		line = self.delcoms(line)
		
		if line == '': return 1
		elif '=' in line:
			var, eq = line.split('=')
			var = var.strip()
			eq = eq.strip()
			
			f = eq[:eq.find(' ')].strip()
			params = eq[eq.find(' '):].strip()
			
			self.vars[var] = eval(f)(params, self, programs)

		else:
			f = line[:line.find(' ')].strip()
			params = line[line.find(' '):].strip()
	
			if f == 'seed':
				random.seed(int(params))
				return
			
			i = f.find('.')
			f = "self.vars['" + f[:i] + "']" + f[i:]
			
			eval(f)(params, self, programs)

class Shell:
	def run(self):
		session = Session()
		while line:=input('$>'):
			session.new(line)

def from_file(file):
	with open(file, 'r') as co:
		lines = co.read().split('\n')

	session = Session()
	for line in lines:
		session.new(line)

if __name__ == '__main__':
	import sys
	from_file(sys.argv[1])
