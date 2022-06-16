import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_angular_distribution(counts, energy, angle, savefile, time, particle):
    fig, ax1 = plt.subplots()
    max_energy = np.max(np.abs(energy))
    cax = ax1.imshow(counts.T, norm=colors.LogNorm(vmin=1E13, vmax=1E20), extent=(angle[0], angle[-1], energy[0], energy[-1]), interpolation='nearest', origin='lower', aspect='auto')
    ax1.set_xlabel('xy angle [rad]')
    ax1.set_ylabel('Energy [MeV]')
    ax1.set_ylim(0, max_energy)
    cbar = fig.colorbar(cax)
    cbar.set_label('dN/dE')
    fig.tight_layout()
    ax1.legend()
    plt.title(particle + ' ' + time + ' ps')
    plt.savefig(savefile, dpi=200)
    plt.close()
