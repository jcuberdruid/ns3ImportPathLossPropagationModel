import numpy as np
import random


path = path = "./src/data/austinBluffsPL.csv"

x = [] 
y = [] 
z = []

with open(path) as file: 
    skip = 3 
    for i in file:
        if (skip == 0):
            array = np.array([i], dtype=str)
            split = np.char.split(array)
            x.append(split[0][1])
            y.append(split[0][2])
            z.append(split[0][0])
        else:
            skip -= 1 

x = np.array(x).astype(float)
y = np.array(y).astype(float)
time = np.array(z).astype(float)

with open("path.tcl", "w") as f:
    setx = f"$node_(0) set X_ {x[0]}"
    sety = f"$node_(0) set Y_ {y[0]}"
    setz = "$node_(0) set Z_ 2"

    f.write(setx)
    f.write("\n")
    f.write(sety)
    f.write("\n")
    f.write(setz)
    f.write("\n")

    for i in range(1, len(x)):
        X = x[i]
        Y = y[i]
        t = time[i]
        line = f"$ns_ at {t} \"$node_(0) setdest {X} {Y} {random.uniform(3,4)}\""
        f.write(line)
        f.write("\n")



