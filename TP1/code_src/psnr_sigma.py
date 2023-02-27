#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
  
x = []
y = []
  
with open('PSNR_graph.txt') as f:
    lines = f.readlines()
    x = [line.split()[0] for line in lines]
    y = [line.split()[1] for line in lines]

  
plt.plot(x, y)
plt.show()