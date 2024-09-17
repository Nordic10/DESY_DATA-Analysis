import pandas as pd
import matplotlib.pyplot as plt

range1 = [0,1,2,3,4,7]
range2 = [4,5,7]

runs = ["186","189","192","194","196","198","200","175","171","167","150","155","158","163","329","331","333","337","340","343","346","347","350","321","319","317","314","311"]

channels1 = [0,1,2,3]
channels2 = [4,5,6,7]

run = runs[14]


#for run in runs:
df = pd.read_csv("run_" + run + "/maxHist_total.csv", header=None)

indexes = df[0]

plt.figure()
for ch in channels1:
    plt.bar(indexes, df[ch+1], label="channel " + str(ch), alpha=0.4, width=4)
plt.legend()
plt.xlabel("Max Voltage [mV]")
plt.ylabel("Count")
plt.ylim(0,800)
plt.xlim(0,750)
plt.title("Max Voltage Histogram")
plt.savefig("run_" + run + "/maxHist_ch0-3.png")

plt.figure()
for ch in channels2:
    plt.bar(indexes, df[ch+1], label="channel " + str(ch), alpha=0.4, width=4)
plt.legend()
plt.xlabel("Max Voltage [mV]")
plt.ylabel("Count")
plt.ylim(0,10000)
plt.xlim(0,750)
plt.title("Max Voltage Histogram")
plt.savefig("run_" + run + "/maxHist_ch4-7.png")

plt.show()
