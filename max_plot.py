import pandas as pd
import matplotlib.pyplot as plt

range1 = [0,1,2,3]
range2 = [4,5,7]

plt.figure()
for channel in range1:
	df = pd.read_csv('run_178/sum_max_ch'+str(channel)+'.csv', header=None)
	plt.plot(df[0], df[1], label='channel '+str(channel), alpha = 0.7)

plt.legend()
plt.title("Normalized Amplitude Pulses")
plt.xlabel("t [ns]")
plt.ylabel("Voltage [mV]")
 
plt.figure()
for channel in range2:
	df = pd.read_csv('run_178/sum_max_ch'+str(channel)+'.csv', header=None)
	plt.plot(df[0], df[1], label='channel '+str(channel), alpha = 0.7)

plt.legend()
plt.title("Normalized Amplitude Pulses")
plt.xlabel("t [ns]")
plt.ylabel("Voltage [mV]")

plt.show()
