# Code written by Serena Fattori
# for any issue write to: dr.serena.fattori@gmail.com


# Import of main libraries
# ========================

from scipy import stats
import scipy.stats.distributions
import matplotlib.pyplot as plt

import numpy
import numpy as np
import os
import math

# pandas libraries are useful to read a text file like cvs.
import pandas as pd

# To use matlab functions
import matplotlib.mlab as mlab
import matplotlib
matplotlib.get_backend()
import matplotlib.colors as mcolors
# To perform polynomial fit
import numpy.polynomial.polynomial as poly

# To integrate funtions and vectors
import scipy.integrate as integrate

from usefulFunctions import getColumns

import pandas as pd
import plotly.express as px
from utils import buildDFfromFile
from utils import pdgToParticle
import matplotlib.pyplot as plt


# Read fils CATANA  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
filename = r'EXP_Data/MitraAlpha_f25_ripple_2.txt'
df_CATANA = buildDFfromFile(filename)
df_CATANA = df_CATANA.apply(pd.to_numeric)
depth_CATANA_f25_ripple_2 = df_CATANA['depth'] / 1.18
dose_CATANA_f25_ripple_2 = df_CATANA['dose'] / df_CATANA['dose'].max()

filename = r'EXP_Data/MitraAlpha_f25_ripple.txt'
df_CATANA = buildDFfromFile(filename)
df_CATANA = df_CATANA.apply(pd.to_numeric)
depth_CATANA_f25_ripple = df_CATANA['depth'] / 1.18
dose_CATANA_f25_ripple = df_CATANA['dose'] / df_CATANA['dose'].max()

filename = r'EXP_Data/MitraAlpha_f25.txt'
df_CATANA = buildDFfromFile(filename)
df_CATANA = df_CATANA.apply(pd.to_numeric)
depth_CATANA_f25 = df_CATANA['depth'] / 1.18
dose_CATANA_f25 = df_CATANA['dose'] / df_CATANA['dose'].max()

filename = r'EXP_Data/MitraAlpha.txt'
df_CATANA = buildDFfromFile(filename)
df_CATANA = df_CATANA.apply(pd.to_numeric)
depth_CATANA = df_CATANA['depth'] / 1.18
dose_CATANA = df_CATANA['dose'] / df_CATANA['dose'].max()
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


# READ FILE PAPER ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
filename = r'EXP_Data/MITRA-Dose-Profile_DataThief.txt'
df_Data_Thief = buildDFfromFile(filename)
df_Data_Thief = df_Data_Thief.apply(pd.to_numeric)
depth_Data_Thief = df_Data_Thief['depth']
dose_Data_Thief = df_Data_Thief['dose']
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# READ FILE YF AND YD MITRA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
filename = r'EXP_Data/He4_SOBP_MITRA_YF_YD.txt'
df_MITRA_YF_YD_SOBP = buildDFfromFile(filename)
df_MITRA_YF_YD_SOBP = df_MITRA_YF_YD_SOBP.apply(pd.to_numeric)
depth_MITRA_YF_YD_SOBP = df_MITRA_YF_YD_SOBP['depth']
YD_MITRA_SOBP = df_MITRA_YF_YD_SOBP['YD']
YF_MITRA_SOBP = df_MITRA_YF_YD_SOBP['YF']

print(depth_MITRA_YF_YD_SOBP)
print(YF_MITRA_SOBP)
print(YD_MITRA_SOBP)
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# READ MC FILES  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Dose
filename = r'MC_Data/MC_He4-MITRA_SOBP_1-RIPPLE_Dose_PHP_1e7.out'
df_Data_MC = buildDFfromFile(filename)
df_Data_MC = df_Data_MC.apply(pd.to_numeric)
depth_Data_MC = df_Data_MC['i'] /10. / 1.18
dose_Data_MC = df_Data_MC['Dose(Gy)'] / df_Data_MC['Dose(Gy)'].max()


# Let
filename = r'MC_Data/MC_He4-MITRA_SOBP_1-RIPPLE_Let_PHP_1e7.out'
df_Data_MC = buildDFfromFile(filename)
df_Data_MC = df_Data_MC.apply(pd.to_numeric)
LDT_Data_MC = df_Data_MC['LDT']
LTT_Data_MC = df_Data_MC['LTT']
alpha_1_D_Data_MC = df_Data_MC['alpha_1_D']
alpha_1_T_Data_MC = df_Data_MC['alpha_1_T']
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


# DRAW PLOTS  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PLOT 1
plt.style.use('seaborn-white')
kwargs = dict(alpha=0.5, bins=75)

figure1 = plt.figure(1, figsize=(13, 6.5))
plt.subplots_adjust(top=0.95, bottom=0.08, left=0.08, right=0.95, wspace=0.3, hspace=0.5)

plt.subplot(221)
plt.scatter(depth_CATANA_f25_ripple_2, dose_CATANA_f25_ripple_2,   label = 'MitraAlpha_f25_ripple_2', color='r', s=10)
plt.scatter(depth_Data_Thief, dose_Data_Thief,   label = 'PAPER', color='b', s=10)
plt.title('MITRA DOSE PROFILE')
plt.xlabel('Depth iN PMMA [mm]')
plt.ylabel('Dose [a.u.]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()

plt.subplot(222)
plt.scatter(depth_CATANA_f25_ripple, dose_CATANA_f25_ripple,   label = 'MitraAlpha_f25_ripple', color='r', s=10)
plt.scatter(depth_Data_Thief, dose_Data_Thief,   label = 'PAPER', color='b', s=10)
plt.title('MITRA DOSE PROFILE')
plt.xlabel('Depth iN PMMA [mm]')
plt.ylabel('Dose [a.u.]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()

plt.subplot(223)
plt.scatter(depth_CATANA_f25, dose_CATANA_f25,   label = 'MitraAlpha_f25', color='r', s=10)
plt.scatter(depth_Data_Thief, dose_Data_Thief,   label = 'PAPER', color='b', s=10)
plt.title('MITRA DOSE PROFILE')
plt.xlabel('Depth iN PMMA [mm]')
plt.ylabel('Dose [a.u.]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()

plt.subplot(224)
plt.scatter(depth_CATANA, dose_CATANA,   label = 'MitraAlpha', color='r', s=10)
plt.scatter(depth_Data_Thief, dose_Data_Thief,   label = 'PAPER', color='b', s=10)
plt.title('MITRA DOSE PROFILE')
plt.xlabel('Depth iN PMMA [mm]')
plt.ylabel('Dose [a.u.]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# PLOT 2
figure2 = plt.figure(2, figsize=(10, 6))
plt.scatter(depth_CATANA_f25_ripple_2, dose_CATANA_f25_ripple_2,   label = 'MitraAlpha_f25_ripple_2', color='r', s=10)
plt.scatter(depth_Data_Thief, dose_Data_Thief,   label = 'PAPER', color='b', s=10)
plt.scatter(depth_Data_MC, dose_Data_MC,   label = 'MC 10.06.p01 - 1e7', color='black', s=10)
plt.title('MITRA DOSE PROFILE')
plt.xlabel('Depth in PMMA [mm]')
plt.ylabel('Dose [a.u.]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



# PLOT 3
figure3 = plt.figure(3, figsize=(10, 6))
plt.scatter(depth_MITRA_YF_YD_SOBP, YD_MITRA_SOBP,   label = 'EXP YD', color='black', s=10)
plt.plot(depth_Data_MC, LDT_Data_MC,   label = 'Let Dose Total', color='r')
plt.plot(depth_Data_MC, alpha_1_D_Data_MC,  label = 'Let Dose Primary',  color='b',)
plt.title('MITRA LET DOSE - MC 10.06.p01- 1e7')
plt.xlabel('Depth in PMMA [mm]')
plt.ylabel('Let [keV/um]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()

# PLOT 4
figure4 = plt.figure(4, figsize=(10, 6))
plt.scatter(depth_MITRA_YF_YD_SOBP, YF_MITRA_SOBP,   label = 'EXP YF', color='black', s=10)
plt.plot(depth_Data_MC, LTT_Data_MC,   label = 'Let Track Total', color='r')
plt.plot(depth_Data_MC, alpha_1_T_Data_MC, label = 'Let Track Primary', color='b')
plt.title('MITRA LET TRACK - MC 10.06.p01 - 1e7')
plt.xlabel('Depth in PMMA [mm]')
plt.ylabel('Let [keV/um]')
plt.legend(fontsize=14, loc='upper left',bbox_to_anchor=(0, 1), prop={'size':14})
x_ticks = np.arange(0, 40, 5)
y_ticks = np.arange(0, 70, 10)
plt.xticks(x_ticks)
#plt.yticks(y_ticks)
plt.grid(True)
plt.draw()


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

plt.show()


