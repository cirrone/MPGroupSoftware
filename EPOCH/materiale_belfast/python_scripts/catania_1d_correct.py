import sdf_helper as sh
import numpy as np
import matplotlib.pyplot as plt
from cv2 import VideoWriter, VideoWriter_fourcc
import matplotlib.colors as colors
from make_SDF_movie import *
import sys

plt.rcParams['axes.labelsize'] = 16
plt.rcParams['font.size'] = 16

initialfile = 1
finalfile = 25

SimulationName = 'pre_plasma' #name of the folder
outputfolder = ('/home/beatrice/epoch/epoch1d/Data/' + SimulationName + '/') # folder path

n_crit = 1.75E27
make_movie = 'Y'
peak_of_pulse = 133 # time where the peak hits x=0

def plottogether(Ey, electron_density, proton_density, x, savefile, time):
    fig, ax1 = plt.subplots()
    color = 'tab:blue'
    ax1.set_xlabel('x($\mu m$)')
    ax1.set_ylabel('$n_e/n_c$', color=color)
    ax1.semilogy(x * 1E6, electron_density, color='k')
    ax1.semilogy(x * 1E6, proton_density, color=color)
    ax1.set_ylim([1E-2, 3E2])
    ax1.set_xlim([-20, 20])
    position = [0.15, 0.15, 0.7, 0.77]
    ax1.set_position(position)
    ax2 = ax1.twinx()
    color = 'tab:red'
    ax2.set_ylabel('Ey $(V/m)$', color=color)
    ax2.plot(x * 1E6, Ey, color=color)
    ax2.set_position(position)
    ax2.set_ylim([-6E13, 6E13])
    ax2.set_xlim([-10, 10])
    plt.title(time + ' fs')
    plt.savefig(savefile, dpi=200)
    plt.close()


def spectrum_plot(proton_energy, proton_spectrum, electron_energy, electron_spectrum, savefile, time):
    fig, ax1 = plt.subplots()
    ax1.plot(proton_energy, proton_spectrum, label='H$^+$')
    ax1.plot(electron_energy, electron_spectrum, label='e$^-$')
    ax1.set_xlabel('Energy')
    ax1.set_ylabel('dN/dE')
    ax1.set_yscale('log')
    ax1.set_ylim(1E18, 1E22)
    ax1.legend()
    plt.title(time + ' fs')
    plt.savefig(savefile, dpi=200)
    plt.close()


def plot_dist_fn(counts, energy, x, savefile, time):
    fig, ax1 = plt.subplots()
    x = x*1E6
    energy = energy/1.6E-13
    cax = ax1.imshow(counts.T, norm=colors.LogNorm(vmin=1E13, vmax=1E20), extent=(x[0], x[-1], energy[0], energy[-1]),
                    interpolation='nearest', origin='lower', aspect='auto')
    ax1.set_xlim(-5, 10)
    ax1.set_xlabel('x ($\mu m$)')
    ax1.set_ylabel('Energy (MeV)')
    cbar = fig.colorbar(cax)
    cbar.set_label('dN/dE')
    plt.title(time + ' fs')
    fig.tight_layout()
    plt.savefig(savefile, dpi=200)
    plt.close()


def plot_electron_phase(counts, px, x, savefile, time):
    fig, ax1 = plt.subplots()
    x = x*1E6
    px = px/(9.11E-31*3E8)
    max_px = np.max(np.abs(px))
    cax = ax1.imshow(counts.T, norm=colors.LogNorm(vmin=1E13, vmax=1E20), extent=(x[0], x[-1], px[0], px[-1]),
                    interpolation='nearest', origin='lower', aspect='auto')
    ax1.set_xlim(-5, 10)
    ax1.set_ylim(-max_px, max_px)
    ax1.set_xlabel('x ($\mu m$)')
    ax1.set_ylabel('px [m$_e$c]')
    cbar = fig.colorbar(cax)
    cbar.set_label('dN/dE')
    plt.title(time + ' fs')
    fig.tight_layout()
    plt.savefig(savefile, dpi=200)
    plt.close()


for filenumber in range(initialfile, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    
    # this defines the image it will save for each thing that we want to plot 
    laserdensityfile = filename_string + 'laserdensity.png'
    dist_fn_file = filename_string + 'proton_phase_space.png'
    ionspectra = filename_string + 'ionspectra.png'
    electronphase = filename_string + 'electron_x_px.png'

    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    x = np.array(data.Grid_Grid_mid.data[0]) # x grid

    time = str(round((data.Header['time'] * 1E15)-peak_of_pulse, 3))
    # extract densities and fields
    electron_density = data.Derived_Number_Density_electron.data/n_crit
    proton_density = data.Derived_Number_Density_proton.data/n_crit
    Ey = data.Electric_Field_Ey.data
    
    plottogether(Ey, electron_density, proton_density, x, laserdensityfile, time) #plot laser and densities
        
    #plot phase space (position vs kinetic energy)
    proton_dist_fn = data.dist_fn_x_energy_proton.data
    dist_fn_energy = data.dist_fn_x_energy_proton.grid.data[1]
    plot_dist_fn(proton_dist_fn, dist_fn_energy, x, savefile=dist_fn_file, time=time)

    electron_dist_fn = data.dist_fn_x_px_electron.data
    dist_fn_px = data.dist_fn_x_px_electron.grid.data[1]
    plot_electron_phase(electron_dist_fn, dist_fn_px, x, savefile=electronphase, time=time)
        
    #plot spectra only
    proton_energy = np.array(data.Grid_energy_spectra_proton.data)/1.6E-13
    proton_spectrum = data.dist_fn_energy_spectra_proton.data

    electron_energy = np.array(data.Grid_energy_spectra_electron.data)/1.6E-13
    electron_spectrum = data.dist_fn_energy_spectra_electron.data

    spectrum_plot(proton_energy[0, :], proton_spectrum, electron_energy[0, :], electron_spectrum, ionspectra, time)

# make movies of the images you created
SDF_movie(outputfolder, SimulationName, initialfile, finalfile, 'laserdensity')
SDF_movie(outputfolder, SimulationName, initialfile, finalfile, 'proton_phase_space')
SDF_movie(outputfolder, SimulationName, initialfile, finalfile, 'ionspectra')
SDF_movie(outputfolder, SimulationName, initialfile, finalfile, 'electron_x_px')

