import sdf_helper as sh
import numpy as np
import matplotlib.pyplot as plt
from cv2 import VideoWriter, VideoWriter_fourcc
import matplotlib.colors as colors
from make_SDF_movie import *
from plot_density_profile import *
from plot_ion_phase_space import *
from plot_electron_phase_space import *
from ion_temperature_analysis import *
from electron_temperature_analysis import *
from plot_angular_distribution import *
import sys
import csv
#from scipy.optimize import curve_fit
import plotly.graph_objs as go
from pylab import *
#import chart_studio.plotly as py
from lmfit.models import ExpressionModel

plt.rcParams['axes.labelsize'] = 16
plt.rcParams['font.size'] = 16

# SIMULATION SETTINGS - FOLDER

initialfile = 0
finalfile = 50

SimulationName = 'Al_proton' #name of the folder
outputfolder = ('/home/giuliana/epoch/epoch2d/Data/belyaev2022/' + SimulationName + '/') # folder path

# SIMULATION SETTINGS - CONSTANTS

n_crit = 1.1E27
make_movie = 'Y'
peak_of_pulse = 0 # time where the peak hits x=0

# SIMULATION SETTINGS - SPECIES FINAL FILE

for filenumber in range(0, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    electron_Al_finalfile = finalfile    
    try:
        x = data.dist_fn_x_px_electron_Al.grid.data[1]
    except:
        electron_Al_finalfile = filenumber 
        break

for filenumber in range(0, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    Al_finalfile = finalfile    
    try:
        x = data.dist_fn_x_energy_aluminum.grid.data[1]
    except:
        Al_finalfile = filenumber 
        break

for filenumber in range(0, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    electron_H_finalfile = finalfile    
    try:
        x = data.dist_fn_x_px_electron_H.grid.data[1]
    except:
        electron_H_finalfile = filenumber 
        break

for filenumber in range(0, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    proton_finalfile = finalfile    
    try:
        x = data.dist_fn_x_energy_proton.grid.data[1]
    except:
        proton_finalfile = filenumber 
        break

# READING AND ANALYZE .SDF FILES

for filenumber in range(initialfile, finalfile+1):
    filename_string = outputfolder + f'{filenumber:04d}'
    filename = (filename_string + '.sdf')
    
    # this defines the image name it will save for each thing that we want to plot 
    densityprofile = filename_string + 'density_profile.png'

    ionphasespace_Al = filename_string + 'Al_phase_space.png'
    ionphasespace_p = filename_string + 'p_phase_space.png'
    electronphasespace_Al = filename_string + 'electron_Al_phase_space.png'
    electronphasespace_H = filename_string + 'electron_H_phase_space.png'

    ionspectra_Al = filename_string + 'ionspectra_Al.png'
    ionspectra_e_Al = filename_string + 'ionspectra_e_Al.png'
    ionspectra_p = filename_string + 'ionspectra_p.png'
    ionspectra_e_H = filename_string + 'ionspectra_e_H.png'
    
    spread_e_Al = filename_string + 'angular_distribution_electron_Al.png'
    spread_Al = filename_string + 'angular_distribution_Al.png'
    spread_e_H = filename_string + 'angular_distribution_electron_H.png'
    spread_p = filename_string + 'angular_distribution_p.png'

    # get data
    data = sh.getdata(filename) # function to read in the data
    sh.list_variables(data)
    x = np.array(data.Grid_Grid_mid.data[0]) # x grid

    time = str(round((data.Header['time'] * 1E12)-peak_of_pulse, 3))
    
    # extract densities and fields

    electron_Al_density = (data.Derived_Number_Density_electron_Al.data/n_crit)
    electron_H_density = (data.Derived_Number_Density_electron_H.data/n_crit)
    Al_density = data.Derived_Number_Density_aluminum.data/n_crit
    proton_density = data.Derived_Number_Density_proton.data/n_crit

    Ey = data.Electric_Field_Ey.data
    
    #plot laser and density profiles

    plot_density_profile(Ey, electron_Al_density, Al_density, electron_H_density, proton_density, x, densityprofile, time, 'electron_Al', 'Al', 'electron_H', 'proton') 
        
    #plot ion phase space (position vs kinetic energy)

    if filenumber < Al_finalfile :    
        Al_x = data.dist_fn_x_energy_aluminum.data
        Al_energy = data.dist_fn_x_energy_aluminum.grid.data[1]
        plot_ion_phase_space(Al_x, Al_energy, x, ionphasespace_Al, time, 'Al')

    if filenumber < proton_finalfile :    
        p_x = data.dist_fn_x_energy_proton.data
        p_energy = data.dist_fn_x_energy_proton.grid.data[1]
        plot_ion_phase_space(p_x, p_energy, x, ionphasespace_p, time, 'proton')

    #plot electron phase space (position vs momentum)

    if filenumber < electron_Al_finalfile :
        electron_Al_x = data.dist_fn_x_px_electron_Al.data
        electron_Al_px = data.dist_fn_x_px_electron_Al.grid.data[1]
        plot_electron_phase_space(electron_Al_x, electron_Al_px, x, electronphasespace_Al, time, 'electron_Al')

    if filenumber < electron_H_finalfile :
        electron_H_x = data.dist_fn_x_px_electron_H.data
        electron_H_px = data.dist_fn_x_px_electron_H.grid.data[1]
        plot_electron_phase_space(electron_H_x, electron_H_px, x, electronphasespace_H, time, 'electron_H')

    #plot spectra (dN/dE vs kinetic energy)
   
    time_step = round((data.Header['time'] * 1E15) - peak_of_pulse, 3)

    if filenumber < Al_finalfile :
        Al_energy_spectrum = np.array(data.Grid_energy_spectra_aluminum.data)/1.6E-13
        Al_dNdE_spectrum = data.dist_fn_energy_spectra_aluminum.data
        ion_temperature_analysis(Al_energy_spectrum[0,:], Al_dNdE_spectrum, ionspectra_Al, time, outputfolder + 't-timestep-Al.csv', 'Al', time_step)

    if filenumber < proton_finalfile :
        proton_energy_spectrum = np.array(data.Grid_energy_spectra_proton.data)/1.6E-13
        proton_dNdE_spectrum = data.dist_fn_energy_spectra_proton.data
        ion_temperature_analysis(proton_energy_spectrum[0,:], proton_dNdE_spectrum, ionspectra_p, time, outputfolder + 't-timestep-p.csv', 'proton', time_step)
        
    if filenumber < electron_Al_finalfile :
        electron_Al_energy_spectrum = np.array(data.Grid_energy_spectra_electron_Al.data)/1.6E-13
        electron_Al_dNdE_spectrum = data.dist_fn_energy_spectra_electron_Al.data
        electron_temperature_analysis(electron_Al_energy_spectrum[0,:], electron_Al_dNdE_spectrum, ionspectra_e_Al, time, outputfolder + 't-timestep-e_Al.csv', 'electron_Al', time_step) 

    if filenumber < electron_H_finalfile :
        electron_H_energy_spectrum = np.array(data.Grid_energy_spectra_electron_H.data)/1.6E-13
        electron_H_dNdE_spectrum = data.dist_fn_energy_spectra_electron_H.data
        electron_temperature_analysis(electron_H_energy_spectrum[0,:], electron_H_dNdE_spectrum, ionspectra_e_H, time, outputfolder + 't-timestep-e_H.csv', 'electron_H', time_step) 

    #plot angular divergence
    if filenumber < Al_finalfile :
        Al_angle = data.Grid_divergence_aluminum.data[0]      
        Al_counts = data.dist_fn_divergence_aluminum.data
        Al_energy_spread = data.dist_fn_divergence_aluminum.grid.data[1]/1.6E-13    
        plot_angular_distribution(Al_counts, Al_angle, Al_energy_spread, spread_Al, time, 'Al')

    if filenumber < proton_finalfile :
        proton_angle = data.Grid_divergence_proton.data[0]      
        proton_counts = data.dist_fn_divergence_proton.data
        proton_energy_spread = data.dist_fn_divergence_proton.grid.data[1]/1.6E-13    
        plot_angular_distribution(proton_counts, proton_angle, proton_energy_spread, spread_p, time, 'proton')

    if filenumber < electron_Al_finalfile :
        electron_Al_angle = data.Grid_divergence_electron_Al.data[0]
        electron_Al_counts = data.dist_fn_divergence_electron_Al.data
        electron_Al_energy_spread = data.dist_fn_divergence_electron_Al.grid.data[1]/1.6E-13
        plot_angular_distribution(electron_Al_counts, electron_Al_angle, electron_Al_energy_spread, spread_e_Al, time, 'electron_Al')

    if filenumber < electron_H_finalfile :
        electron_H_angle = data.Grid_divergence_electron_H.data[0]
        electron_H_counts = data.dist_fn_divergence_electron_H.data
        electron_H_energy_spread = data.dist_fn_divergence_electron_H.grid.data[1]/1.6E-13
        plot_angular_distribution(electron_H_counts, electron_H_angle, electron_H_energy_spread, spread_e_H, time, 'electron_H')

# make movies of the images you created
SDF_movie(outputfolder, SimulationName, 0, finalfile, 'density_profile')
SDF_movie(outputfolder, SimulationName, 0, Al_finalfile, 'Al_phase_space')
SDF_movie(outputfolder, SimulationName, 0, proton_finalfile, 'p_phase_space')
SDF_movie(outputfolder, SimulationName, 0, electron_Al_finalfile, 'electron_Al_phase_space')
SDF_movie(outputfolder, SimulationName, 0, electron_H_finalfile, 'electron_H_phase_space')

SDF_movie(outputfolder, SimulationName, 0, Al_finalfile, 'ionspectra_Al')
SDF_movie(outputfolder, SimulationName, 0, electron_Al_finalfile, 'ionspectra_e_Al')
SDF_movie(outputfolder, SimulationName, 0, proton_finalfile, 'ionspectra_p')
SDF_movie(outputfolder, SimulationName, 0, electron_H_finalfile, 'ionspectra_e_H')

SDF_movie(outputfolder, SimulationName, 0, electron_Al_finalfile, 'angular_distribution_electron_Al')
SDF_movie(outputfolder, SimulationName, 0, electron_H_finalfile, 'angular_distribution_electron_H')
SDF_movie(outputfolder, SimulationName, 0, Al_finalfile, 'angular_distribution_Al')
SDF_movie(outputfolder, SimulationName, 0, proton_finalfile, 'angular_distribution_p')

