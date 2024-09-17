import pandas as pd
import matplotlib.pyplot as plt

for channel in range(8):
	plt.figure()
	for event in range(6):
		df = pd.read_csv('firstSix/ch'+str(channel)+'/total.csv')	
		plt.plot(df['x'+str(event)], df['y'+str(event)], label='event '+str(event))
	plt.legend()
	plt.title("Pulse Samples for Channel " + str(channel))
	plt.xlabel("t [ns]")
	plt.ylabel("Voltage [mV]")

plt.show()
