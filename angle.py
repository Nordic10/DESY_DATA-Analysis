import pandas as pd
import matplotlib.pyplot as plt

#range1 = [0,1,2,3]
#range2 = [4,5,6,7]

def asb():
	for channel in range(8):
		plt.figure()
		df1 = pd.read_csv('run_186/sum_max_ch'+str(channel)+'.csv', header=None)
		df2 = pd.read_csv('run_189/sum_max_ch'+str(channel)+'.csv', header=None)
		df3 = pd.read_csv('run_192/sum_max_ch'+str(channel)+'.csv', header=None)
		df4 = pd.read_csv('run_194/sum_max_ch'+str(channel)+'.csv', header=None)
		df5 = pd.read_csv('run_196/sum_max_ch'+str(channel)+'.csv', header=None)
		df6 = pd.read_csv('run_198/sum_max_ch'+str(channel)+'.csv', header=None)
		df7 = pd.read_csv('run_200/sum_max_ch'+str(channel)+'.csv', header=None)
		plt.plot(df1[0], df1[1], label='-90 deg')
		#plt.fill_between(x=df1[0],y1=df1[1],color='b',alpha=0.2)
		plt.plot(df2[0], df2[1], label='-60 deg')
		plt.plot(df3[0], df3[1], label='-30 deg')
		plt.plot(df4[0], df4[1], label='0 deg')
		plt.plot(df5[0], df5[1], label='30 deg')
		plt.plot(df6[0], df6[1], label='60 deg')
		plt.plot(df7[0], df7[1], label='90 deg')
		plt.legend()
		plt.title("Normalized Pulse Angle Comparison Channel " + str(channel))
		plt.xlabel("t [ns]")
		plt.ylabel("Voltage [mV]")
	plt.show()

def absf():
	for channel in range(8):
		plt.figure()
		df1 = pd.read_csv('run_329/sum_max_ch'+str(channel)+'.csv', header=None)
		df2 = pd.read_csv('run_333/sum_max_ch'+str(channel)+'.csv', header=None)
		df3 = pd.read_csv('run_337/sum_max_ch'+str(channel)+'.csv', header=None)
		df4 = pd.read_csv('run_340/sum_max_ch'+str(channel)+'.csv', header=None)
		df5 = pd.read_csv('run_343/sum_max_ch'+str(channel)+'.csv', header=None)
		df6 = pd.read_csv('run_346/sum_max_ch'+str(channel)+'.csv', header=None)
		df7 = pd.read_csv('run_347/sum_max_ch'+str(channel)+'.csv', header=None)
		df8 = pd.read_csv('run_350/sum_max_ch'+str(channel)+'.csv', header=None)
		plt.plot(df1[0], df1[1], label='-90 deg')
		#plt.fill_between(x=df1[0],y1=df1[1],color='b',alpha=0.2)
		plt.plot(df2[0], df2[1], label='-60 deg')
		plt.plot(df3[0], df3[1], label='-30 deg')
		plt.plot(df4[0], df4[1], label='0 deg')
		plt.plot(df5[0], df5[1], label='30 deg')
		plt.plot(df6[0], df6[1], label='60 deg')
		plt.plot(df7[0], df7[1], label='80 deg')
		plt.plot(df8[0], df8[1], label='90 deg')
		plt.legend()
		plt.title("Normalized Pulse Angle Comparison Channel " + str(channel))
		plt.xlabel("t [ns]")
		plt.ylabel("Voltage [mV]")
	plt.show()
	

def dsb():
	for channel in range(8):
		plt.figure()
		df1 = pd.read_csv('run_175/sum_max_ch'+str(channel)+'.csv', header=None)
		df2 = pd.read_csv('run_171/sum_max_ch'+str(channel)+'.csv', header=None)
		df3 = pd.read_csv('run_167/sum_max_ch'+str(channel)+'.csv', header=None)
		df4 = pd.read_csv('run_150/sum_max_ch'+str(channel)+'.csv', header=None)
		df5 = pd.read_csv('run_155/sum_max_ch'+str(channel)+'.csv', header=None)
		df6 = pd.read_csv('run_158/sum_max_ch'+str(channel)+'.csv', header=None)
		df7 = pd.read_csv('run_163/sum_max_ch'+str(channel)+'.csv', header=None)
		plt.plot(df1[0], df1[1], label='-70 deg')
		#plt.fill_between(x=df1[0],y1=df1[1],color='b',alpha=0.2)
		plt.plot(df2[0], df2[1], label='-60 deg')
		plt.plot(df3[0], df3[1], label='-30 deg')
		plt.plot(df4[0], df4[1], label='0 deg')
		plt.plot(df5[0], df5[1], label='30 deg')
		plt.plot(df6[0], df6[1], label='60 deg')
		plt.plot(df7[0], df7[1], label='70 deg')
		plt.legend()
		plt.title("Normalized Pulse Angle Comparison Channel " + str(channel))
		plt.xlabel("t [ns]")
		plt.ylabel("Voltage [mV]")
	plt.show()

def dsbf():
	for channel in range(8):
		plt.figure()
		df1 = pd.read_csv('run_186/sum_max_ch'+str(channel)+'.csv', header=None)
		df2 = pd.read_csv('run_189/sum_max_ch'+str(channel)+'.csv', header=None)
		df3 = pd.read_csv('run_192/sum_max_ch'+str(channel)+'.csv', header=None)
		df4 = pd.read_csv('run_194/sum_max_ch'+str(channel)+'.csv', header=None)
		df5 = pd.read_csv('run_196/sum_max_ch'+str(channel)+'.csv', header=None)
		df6 = pd.read_csv('run_198/sum_max_ch'+str(channel)+'.csv', header=None)
		df7 = pd.read_csv('run_200/sum_max_ch'+str(channel)+'.csv', header=None)
		plt.plot(df1[0], df1[1], label='-90 deg')
		#plt.fill_between(x=df1[0],y1=df1[1],color='b',alpha=0.2)
		plt.plot(df2[0], df2[1], label='-60 deg')
		plt.plot(df3[0], df3[1], label='-30 deg')
		plt.plot(df4[0], df4[1], label='0 deg')
		plt.plot(df5[0], df5[1], label='30 deg')
		plt.plot(df6[0], df6[1], label='60 deg')
		plt.plot(df7[0], df7[1], label='90 deg')
		plt.legend()
		plt.title("Normalized Pulse Angle Comparison Channel " + str(channel))
		plt.xlabel("t [ns]")
		plt.ylabel("Voltage [mV]")
	plt.show()

def main():
	print("Chose config (abs, absf, dsb, dsbf)")
	config = input("")
	if config == 'abs':
		abs()
	elif config == 'absf':
		absf()
	elif config == 'dsb':
		dsb()
	elif config == 'dsbf':
		dsbf()

main()
