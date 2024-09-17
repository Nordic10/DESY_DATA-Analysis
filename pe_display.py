import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

run = ""

dict = {311:[], 314:[], 317:[], 319:[], 321:[], 350: [], 347: [], 346: [], 343: [], 340: [], 337: [], 333: [], 331: [], 329: [], 178: [], 163: [], 158: [], 155: [], 150: [], 167: [], 171: [], 175: [], 200: [], 198: [], 196: [], 194: [], 192: [], 189: [], 186: []}

f = open("peComp/maxes.txt","r")
for line in f:
    l = line.strip()
    if l[0:3] == "run":
        run = l[4:7]
    if l[0:5] == "Xend:" or l[0:5] == "Yend:":
        dict[int(run)].append(float(l[5:]))

print(dict)


channels = [7]
run = 150
print(dict[run])
for ch in channels:
    df = pd.read_csv('peComp/run'+str(run)+'_ch'+str(ch)+'.csv',header=None)
    der_df = pd.read_csv('peComp/der_run'+str(run)+'_ch'+str(ch)+'.csv',header=None)
    plt.figure()
    plt.subplot(211)
    plt.plot(df[0], df[1])
    plt.title("Normalized Pulse Channel "+str(ch))
    plt.xlabel("t [ns]")
    plt.ylabel("Voltage [mV]")
    
    plt.subplot(212)
    plt.plot(der_df[0], der_df[1])
    plt.title("Derivative of Normalized Pulse Channel"+str(ch))
    plt.xlabel("t [ns]")
    plt.ylabel("dV/dt [mV/ns]")

    plt.subplots_adjust(hspace = .5, wspace = .5)

    plt.figure()
    plt.subplot(211)
    plt.plot(df[0], df[1], label='Pulse')
    plt.axvline(x=dict[run][6], color="red", linestyle="--", label="t="+str(dict[run][6]))
    plt.axvline(x=dict[run][7], color="green", linestyle="--", label="t="+str(dict[run][7]))
    plt.xticks(np.arange(85,300, step=30), labels = [str(i) for i in range(85,300,30)])
    plt.xlim(85,150)
    plt.legend(loc='lower right') #bbox_to_anchor=(.95, 1.0),
    plt.title("Normalized Pulse Channel "+str(ch))
    plt.xlabel("t [ns]")
    plt.ylabel("Voltage [mV]")
    
    plt.subplot(212)
    plt.plot(der_df[0], der_df[1], label='Pulse')
    plt.axvline(x=dict[run][4], color="red", linestyle="--", label="t="+str(dict[run][4]))
    plt.xticks(np.arange(85,300, step=30), labels = [str(i) for i in range(85,300,30)])
    plt.xlim(85,150)
    plt.legend(loc='lower right')
    plt.title("Derivative of Normalized Pulse Channel "+str(ch))
    plt.xlabel("t [ns]")
    plt.ylabel("dV/dt [mV/ns]")
    
    plt.subplots_adjust(hspace = .5, wspace = .5)

plt.show()

