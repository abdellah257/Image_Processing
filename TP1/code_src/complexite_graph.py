#! /usr/bin/env python3
import matplotlib.pyplot as plt



# namefiles = ["comparaison_time/formes2g_fft.txt",
# "comparaison_time/formes2g_gaussien2D.txt",
# "comparaison_time/formes2g_gaussien_separable.txt"
# ]

namefiles = ["complexite_fft.txt",
             "complexite_sep.txt",
             "complexite_2D.txt"
             ]


a = 0

# x axis values
x = []
# corresponding y axis values
y = []
z = []
w = []
file0 = open(namefiles[0], "r")
for line in file0:
    temps, sigma = line.split(' ')
    x.append(float(sigma))
    y.append(float(temps))
file0.close()

file1 = open(namefiles[1], "r")
for line in file1:
    temps, sigma = line.split(' ')
    z.append(float(temps))

file2 = open(namefiles[2], "r")
for line in file2:
    temps, sigma = line.split(' ')
    w.append(float(temps))
file1.close()

plt.plot(x, y, color='r', label='FFT')
plt.plot(x, z, color='g', label='Convolution Séparable')
plt.plot(x, w, color='b', label='Convolution 2D')

plt.legend()

# function to show the plot
plt.show()