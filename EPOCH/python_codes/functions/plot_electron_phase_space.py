import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv

def plot_electron_phase_space(counts, px, x, savefile, time, particle):
    fig, ax1 = plt.subplots()
    x = x*1E6
    px = px/(9.11E-31*3E8)
    max_px = np.max(np.abs(px))
    cax = ax1.imshow(counts.T, norm=colors.LogNorm(vmin=1E13, vmax=1E20), extent=(x[0], x[-1], px[0], px[-1]),
                    interpolation='nearest', origin='lower', aspect='auto')
    ax1.set_xlim([0, 20])
    ax1.set_ylim(-max_px, max_px)
    ax1.set_xlabel('x ($\mu m$)')
    ax1.set_ylabel('px [m$_e$c]')
    cbar = fig.colorbar(cax)
    cbar.set_label('dN/dE')
    plt.title(particle + ' ' + time + ' ps')
    fig.tight_layout()
    plt.savefig(savefile, dpi=200)
    plt.close()
