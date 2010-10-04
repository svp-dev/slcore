#!/usr/bin/python

from sys import argv, exit

filename = ""
strings = [] 

data = []
fields = ["clocks", "n_exec_insns", "n_issued_flops", "n_compl_loads", "n_compl_stores", "n_bytesin_core", "n_bytesout_core", "n_extmem_cl_in", "n_extmem_cl_out", "intervals"]

def help():
	print "script for extracting features from info dumps"
	print "usage: python", argv[0], "<filename>"
	print 

def process(line):
	tokens = line.split()
	if tokens[9] == '"init"':
		pass
	else:
		index = int(tokens[9][1:])
		if len(data) == index:
			entry = {}
		else:
			entry = data[index]
		name = " ".join(tokens[10:])[:-1]
		entry[name] = {}
		for field, datum in zip(fields[:-1], tokens[:-1]):
			entry[name][field] = int(datum)
		if len(data) == index:
			data.append(entry)
		else:
			data[index] = entry

if len(argv) < 2:
	help()
	exit(1)
else:
	try:
		filename = argv[1]
		flag = False
		for line in open(filename, "r").readlines():
			if flag:
				if not line.startswith("###"):
					process(line)
				else:
					flag = False
			elif line.startswith('"clocks"'):
				flag = True
		
		for i in xrange(len(data)):
			entry = data[i]
			# print entry
			#print i, entry["work"]["clocks"] + entry["proc reqs"]["clocks"] + entry["traverse"]["clocks"]
			print i, entry["work"]["clocks"] 


	except Exception, e:
		help()
		print e
		raise
		exit(1)



