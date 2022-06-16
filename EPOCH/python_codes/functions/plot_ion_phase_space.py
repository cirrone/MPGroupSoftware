import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_ion_phase_space(counts, energy, x, savefile, time, particle):
    fig, ax1 = plt.subplots()
    x = x*1E6
    energy = energy/1.6E-13
    cax = ax1.imshow(counts.T, norm=colors.LogNorm(vmin=1E13, vmax=1E20), extent=(x[0], x[-1], energy[0], energy[-1]),
                    interpolation='nearest', origin='lower', aspect='auto')
    ax1.set_xlim([0, 20])
    ax1.set_xlabel('x [$\mu m$]')
    ax1.set_ylabel('Energy [MeV]')
    cbar = fig.colorbar(cax)
    cbar.set_label('dN/dE')
    plt.title(particle+ ' '+ time + ' ps')
    fig.tight_layout()
    plt.savefig(savefile, dpi=200)
    plt.close()

