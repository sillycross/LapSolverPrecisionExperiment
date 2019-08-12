from allresults import result
from allresults2 import result as result2
import matplotlib.pyplot as plt
import math

def AddLine(y, label, transform=None):
	if transform != None:
		y = [transform(val) for val in y]
	plt.plot(range(0, len(y)), y, label=label)

def AddLineWithMinMax(key, label, transform=None):
	avg = key['relres'];
	lb = key['minresvec'];
	ub = key['maxresvec'];
	if transform != None:
		avg = [transform(val) for val in avg]
		ub = [transform(val) for val in ub]
		lb = [transform(val) for val in lb]
	
	tmp = plt.plot(range(0, len(avg)), avg, label=label);
	color = tmp[0].get_color()
	
	plt.fill_between(range(0, len(avg)), lb, ub, alpha=0.4, edgecolor=color, facecolor=color, linewidth=0)
    
    
allDataId = ['grid2_10000_iu1000',
'grid3_27000_iu1000',
'ipm_grid3_8000_10',
'ipm_grid3_8000_20',
'ipm_grid3_8000_50',
'ipm_grid3_8000_70',
'pathDisjoint_1000_exp20',
'rand_1000_iu1000',
'rand_1000_u1000',
'randdense_1000_iu1000',
'randdense_1000_u1000']

for x in allDataId:
	dataId = "%s_default" % x
	AddLineWithMinMax(result2[dataId]['recursiveCg']['double_2norm'], 'Recursive CG - 53', lambda x: math.log(x,10));
	AddLineWithMinMax(result2[dataId]['recursiveCg']['mpfr1024_2norm'], 'Recursive CG - 1024', lambda x: math.log(x,10));
	AddLine(result[dataId]['treePcg']['double_2norm']['relres'], 'Tree PCG - double', lambda x: math.log(x,10));
	AddLine(result[dataId]['treePcg']['mpfr1024_2norm']['relres'], 'Tree PCG - 1024', lambda x: math.log(x,10));

	plt.legend()
	plt.savefig("Tree_vs_AugTree/%s.png" % dataId)
	plt.clf()
	plt.cla()

