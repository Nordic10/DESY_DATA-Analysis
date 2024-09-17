import pandas as pd
import matplotlib.pyplot as plt

range = [3,7]

for channel in range:
	plt.figure()
	df1 = pd.read_csv('run_200/sum_max_ch'+str(channel)+'.csv', header=None)
	df2 = pd.read_csv('run_178/sum_max_ch'+str(channel)+'.csv', header=None)
	plt.plot(df1[0], df1[1], label='Filtered')
	plt.fill_between(x=df1[0],y1=df1[1],color='b',alpha=0.2)
	plt.plot(df2[0], df2[1], label='Unfiltered')
	plt.fill_between(x=df2[0],y1=df2[1],color='y',alpha=0.2)
	plt.legend()
	if (channel == 3):
		plt.title("Normalized Pulse Comparison Channel 3")
	else:
		plt.title("Normalized Pulse Comparison Channel 7")
	plt.xlabel("t [ns]")
	plt.ylabel("Voltage [mV]")


plt.show()
