import pandas as pd
import matplotlib.pyplot as plt

range1 = [0,1,2,3]
range2 = [4,7]

print("run #:")
run = input("")

plt.figure()
for channel in range1:
	df = pd.read_csv('run_' + run + '/sum_avg_ch'+str(channel)+'.csv', header=None)
	plt.plot(df[0], df[1], label='channel '+str(channel))

plt.legend(loc = 'lower right')
plt.title("Average Pulses")
plt.xlabel("t [ns]")
plt.ylabel("Voltage [mV]")


plt.figure()
"""
	df = pd.read_csv('run_' + run + '/sum_avg_ch'+str(channel)+'.csv', header=None)
	plt.plot(df[0], df[1], label='channel '+str(channel))
"""
df = pd.read_csv('run_' + run + '/sum_avg_ch4.csv', header=None)
plt.plot(df[0], df[1], label='channel 4', color='magenta')
df = pd.read_csv('run_' + run + '/sum_avg_ch7.csv', header=None)
plt.plot(df[0], df[1], label='channel 7', color='black')

plt.legend()
plt.title("Average Pulses")
plt.xlabel("t [ns]")
plt.ylabel("Voltage [mV]")


plt.show()
