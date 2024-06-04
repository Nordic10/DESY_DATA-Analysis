import pandas as pd
import matplotlib.pyplot as plt

#for channel in range(8):
df = pd.read_csv('firstSix/ch'+str(0)+'/total.csv')
plt.plot(df['x0'], df['y0'])
plt.show
