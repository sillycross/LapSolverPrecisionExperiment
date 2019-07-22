from Queue import *
from threading import *
import random

all_data = [{
	'data': 'grid2_10000_iu1000',
	'trees': 5
},
{
	'data': 'grid3_27000_iu1000',
	'trees': 5
},
{
	'data': 'ipm_grid3_8000_10',
	'trees': 5
},
{
	'data': 'ipm_grid3_8000_20',
	'trees': 5
},
{
	'data': 'ipm_grid3_8000_50',
	'trees': 5
},
{
	'data': 'ipm_grid3_8000_70',
	'trees': 5
},
{
	'data': 'pathDisjoint_1000_exp20',
	'trees': 2
},
{
	'data': 'rand_1000_iu1000',
	'trees': 5
},
{
	'data': 'rand_1000_u1000',
	'trees': 5
},
{
	'data': 'randdense_1000_iu1000',
	'trees': 5
},
{
	'data': 'randdense_1000_u1000',
	'trees': 5
}
]

def worker():
    while True:
        cmd = q.get()
        print "Running command: %s" % cmd
        res = os.system(cmd)
        if res != 0:
        	print "WARNING: cmd %s failed!!!" % cmd
        q.task_done()

print "Compiling binaries..."
os.system("mkdir executables");
os.system("mkdir results");
for norm in ['_Lnorm', '_2norm']:
	for precision in [0, 64, 128, 256, 512, 1024]:
		os.system("echo '' > config.h");
		if (norm == '_Lnorm'):
			os.system("echo '#define USE_L_NORM' >> config.h")
		if (precision != 0):
			os.system("echo '#define USE_MPFR' >> config.h")
			os.system("echo '#define MPFR_PRECISION %d' >> config.h" % precision)
			precision_text = '_mpfr%d' % precision
		else:
			precision_text = '_double'
		executable = 'main%s%s' % (precision_text, norm)
		print "Building executable %s.." % executable
		os.system('make clean')
		res = os.system('make release')
		if res != 0:
			print "Build failed! Halting..."
			assert False
		os.system('mv main executables/%s' % executable)

print "Binaries compilation successful."

q = Queue()
for i in range(1):
     t = Thread(target=worker)
     t.daemon = True
     t.start()
     
allCmds = []
for data in all_data:
	for tree in range(0, data['trees'] + 1):
		for alg in ['RunTreePcg', 'RunRecursiveCg']:
			for norm in ['_Lnorm', '_2norm']:
				for precision in ['_double', '_mpfr64', '_mpfr128', '_mpfr256', '_mpfr512', '_mpfr1024']:
					executable = 'main%s%s' % (precision, norm);
					dataTag = data['data'];
					if tree == 0:
						dataTag += '_default'
					else:
						dataTag += '_tree%d' % tree
					dataPath = 'data/%s/graph.mtx' % data['data']
					cmd = './executables/%s --gtest_filter=Benchmark.%s %s %s' % (executable, alg, dataTag, dataPath)
					if (tree > 0):
						treePath = 'data/%s/tree%d.mtx' % (data['data'], tree)
						cmd += ' ' + treePath
					allCmds.append(cmd)
				
random.shuffle(allCmds)
for cmd in allCmds:
	q.put(cmd)
		
q.join()


