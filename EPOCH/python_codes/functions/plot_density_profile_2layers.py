import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_density_profile(Ey, density1, density2, density3, density4, x, savefile, time, particle1, particle2, particle3, particle4):
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel('x [$\mu m$]')
    ax1.set_ylabel('$n/n_c$', color=color)
    ax1.semilogy(x * 1E6, density1, color='k', label=particle1)
    ax1.semilogy(x * 1E6, density2, color='tab:green', label=particle2)
    ax1.semilogy(x * 1E6, density3, color='tab:orange', label=particle3)
    ax1.semilogy(x * 1E6, density4, color='tab:purple', label=particle4)
    ax1.set_ylim([1E-2, 3E2])
    ax1.set_xlim([0, 20])
    position = [0.15, 0.15, 0.7, 0.77]
    ax1.set_position(position)
    ax2 = ax1.twinx()
    color = 'tab:red'
    ax2.set_ylabel('Ey $(V/m)$', color=color)
    ax2.plot(x * 1E6, Ey, color=color, label='E$_y$')
    ax2.set_position(position)
    ax2.set_ylim([-6E13, 6E13])
    ax2.set_xlim([0, 40])
    plt.title(time + ' ps')
    plt.savefig(savefile, dpi=200)
    plt.close()
