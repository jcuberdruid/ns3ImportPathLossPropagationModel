import numpy as np
import random


path = path = "./src/data/austinBluffsPL.csv"

x = [] 
y = [] 
pl = []

with open(path) as file: 
    skip = 3 
    for i in file:
        if (skip == 0):
            array = np.array([i], dtype=str)
            split = np.char.split(array)
            x.append(split[0][1])
            y.append(split[0][2])
            pl.append(split[0][5])
        else:
            skip -= 1 

x = np.array(x).astype(float)
y = np.array(y).astype(float)
pl = np.array(pl).astype(float)

with open("plCSV.csv", "w") as f:

    for i in range(0, len(x)):
        X = x[i]
        Y = y[i]
        p = pl[i]
        line = f"{X},{Y},{p}"
        f.write(line)
        f.write("\n")



