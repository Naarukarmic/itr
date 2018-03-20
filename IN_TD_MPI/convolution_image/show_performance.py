#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

#Process 1
with open("calcul1.txt") as f:
    data = f.read()
x1 = [float(row) for row in data.split('\n')]
t1 = range(0, len(x1));

#Process 2
with open("calcul2.txt") as f:
    data = f.read()
x2 = [float(row) for row in data.split('\n')]
t2 = range(0, len(x1));

#Process 3
with open("calcul3.txt") as f:
    data = f.read()
x3 = [float(row) for row in data.split('\n')]
t3 = range(0, len(x1));

#Process 4
with open("calcul4.txt") as f:
    data = f.read()
x4 = [float(row) for row in data.split('\n')]
t4 = range(0, len(x1));


fig = plt.figure(1)
fig.patch.set_facecolor('cyan')

#First figure with all the different measured time for the convolution
plt.subplot(211)
plt.plot(t1 , x1 , label='1 core')
plt.plot(t2 , x2 , label='2 cores')
plt.plot(t3 , x3 , label='3 cores')
plt.plot(t4 , x4, label='4 cores')
plt.xlabel('Number of calculation')
plt.ylabel('measured times (ms)')
plt.title('Time variation with multiple execution')
plt.legend(bbox_to_anchor=(0.98, 1), loc=2, borderaxespad=0.)
plt.grid(True)

plt.subplot(212)
nb_process = range(1,5)
mean_measured_time = [sum(x1)/len(x1),sum(x2)/len(x2),sum(x3)/len(x3),sum(x4)/len(x4)]
plt.plot(nb_process , mean_measured_time , 'b' , nb_process , mean_measured_time ,'ro')
plt.xlabel('Number of processes')
plt.ylabel('average computed ime')
plt.title('Time variation with the number of process')
plt.grid(True)

plt.show()