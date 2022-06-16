import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_spectrum(energy, spectrum, savefile, time, particle):
    fig, ax1 = plt.subplots()
    ax1.plot(energy, spectrum, label=particle)
    ax1.set_xlabel('Energy [MeV]')
    ax1.set_ylabel('dN/dE')
    ax1.set_yscale('log')
    plt.title(time + ' ps')
    plt.savefig(savefile, dpi=200)
    plt.close()
