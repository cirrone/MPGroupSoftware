import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_spectrum_with_fit(energy, spectrum, savefile, time, y_fit, x_fit, particle):
    fig, ax1 = plt.subplots()
    ax1.plot(energy, spectrum, label=particle)
    ax1.plot(x_fit, y_fit, label='fit')
    ax1.set_xlabel('Energy [MeV]')
    ax1.set_ylabel('dN/dE')
    ax1.set_yscale('log')
    ax1.legend()
    plt.title(time + ' ps')
    plt.savefig(savefile, dpi=200)
    plt.close()
