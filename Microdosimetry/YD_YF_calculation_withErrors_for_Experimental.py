# GAP Cirrone.
# May 3rd, 2021 - Created
# May 3rd, 2021 - Adds of the errors to the quantities calculations
# May 17th, 2021 - Added the section related to the calculation of spectra and
# quantities from the experimental spectra
#
# Calculation of the microdosimetric quantities YF and YD from an energy spectra
# either coming from an experimental run (e.g.
# a spectra from a MCA).
#
# The calculation propagates errors the User may want consider
#
# INPUT:
# A one-column file containing:
#           First column: the counts in each bin as written in a MCA
#
import numpy

import numpy as np
import matplotlib.pyplot as plt

# Libraies for error propagation
#
from uncertainties import ufloat
from uncertainties.umath import *
import uncertainties.unumpy as unumpy

# ************************************************
# ************************************************
#
# Analysis of the experimental data
#
# Open the file spectra from the multi channel analyser. In this case
# the vector representing the channel must be generated according to the
# proper calibration
#
# The MCA returns a vector of 8192 channels (from the channel 0 to the channel 8191)
#
fileSpectraFromMCA = open('Gaf82_10um_2pA_FE.txt', 'r')
lines_fileSpectraFromMCA = fileSpectraFromMCA.readlines()
fileSpectraFromMCA.close()

countsFromFileSpectraFromMCA = []

# Loop on all the file lines and slit all the columns
#
for line in lines_fileSpectraFromMCA:
    p = line.split()
# finally vectors are created
    countsFromFileSpectraFromMCA.append(float(p[0]))

# Created vectors are transformed in Numpy vectors
#
# energyBinFromFileSpectraFromMCA = np.array(energyBinFromFileSpectraFromMCA)
countsFromFileSpectraFromMCA = np.array(countsFromFileSpectraFromMCA)

# The energy bin must be constructed starting from the channel number and applying the proper
# calibration
#
# In this case the used calibration is:
#
# x is the channel
# E is the energy bin [ keV ]
#
# E = p1*x + p2
# p1 = 2.907(2.907, 2.907)
# p2 = -0.05026(-0.1993, 0.09875)
#
maximumChannel = 8191
channelFromFileSpectraFromMCA = np.linspace(0, maximumChannel, maximumChannel+1)

# Applying the calibration
#
# Calibration factors with the errors calculated from the fit
#
p1WithErrors = unumpy.uarray(2.907,0)  # Error on p1 is very small and it is assumed zero
p2WithErrors = unumpy.uarray(-0.05026, 0.03463)

# Conversion of the channel in an energy bin
#
energyBinFromFileSpectraFromMCAWithErrors = channelFromFileSpectraFromMCA*p1WithErrors + p2WithErrors

# The errors on counts N can be the sqrt(N) of the counts in each bin
#
errorCountsFromMCA = np.sqrt(countsFromFileSpectraFromMCA)
countsFromFileSpectraFromMCAWithErrors = unumpy.uarray(countsFromFileSpectraFromMCA, errorCountsFromMCA)

# Value of the mean chord
#
meanChordExperimental = 17.24
meanChordExperimentalError = 0.1
meanChordExperimentalWithErrors = unumpy.uarray(meanChordExperimental, meanChordExperimentalError)

# Create a vector dividing each energy bin by the mean chord
#
y_Exp = energyBinFromFileSpectraFromMCAWithErrors/meanChordExperimentalWithErrors

# Let define Fy the counts or frequencies
#
Fy_Exp = countsFromFileSpectraFromMCAWithErrors

# Calculate the integral of (y_Exp,Fy_Exp)
#
integral_y_Fy_Exp = np.trapz(Fy_Exp,y_Exp)

# Normalisation of Fy as respect the integral
#
Fy_normalised_Exp = Fy_Exp/integral_y_Fy_Exp

# ***** Calculation of yF
#
yF_Exp = np.trapz(y_Exp*Fy_normalised_Exp, y_Exp)

# ***** Calculation of yD
#
# Firstly, we must calculate the dose distribution d(y)
#
dy_Exp = (y_Exp*Fy_normalised_Exp/yF_Exp)

yD_Exp = np.trapz(y_Exp*dy_Exp,y_Exp)

# Extraction of the medium value and corresponding standard deviation  of
# the various quantities we have calculated
#
# Extraction of the energy vector
#
energyBinFromFileSpectraFromMCAWithErrors_Value = unumpy.nominal_values(energyBinFromFileSpectraFromMCAWithErrors)
energyBinFromFileSpectraFromMCAWithErrors_StandardDeviation = unumpy.std_devs(energyBinFromFileSpectraFromMCAWithErrors)

# Extraction of the nominal values and of the standard deviation of all the vectors
#
y_Value_Exp = unumpy.nominal_values(y_Exp)
y_StandardDeviation_Exp = unumpy.std_devs(y_Exp)

Fy_Value_Exp = unumpy.nominal_values(Fy_Exp)
Fy_StandardDeviation_Exp = unumpy.std_devs(Fy_Exp)

Fy_normalised_Value_Exp = unumpy.nominal_values(Fy_normalised_Exp)
Fy_normalised_StandardDeviation_Exp = unumpy.std_devs(Fy_normalised_Exp)

yF_Value_Exp = unumpy.nominal_values(yF_Exp)
yF_StandardDeviation_Exp = unumpy.std_devs(yF_Exp)



# calculation of yd(y)
#
y_dyWithErrors_Exp = y_Exp*y_Exp*Fy_normalised_Exp/yF_Value_Exp
# Extraction of Value and Standard deviation of yd(y)
#
y_dy_Value_Exp = unumpy.nominal_values(y_dyWithErrors_Exp)
y_dy_StandardDeviation_Exp = unumpy.std_devs(y_dyWithErrors_Exp)

print(yF_Exp)
print(yD_Exp)


# ***********************************************************************
# PLOTS
#

# Energy spectra
#
# Experimental data
#
figure20 = plt.figure(20, figsize=(6, 6))
plt.subplot(2,2,1)
plt.errorbar(energyBinFromFileSpectraFromMCAWithErrors_Value, Fy_Value_Exp,
             Fy_StandardDeviation_Exp,
             energyBinFromFileSpectraFromMCAWithErrors_StandardDeviation,
             marker=".", fillstyle='none',
             linestyle='none',
             markeredgecolor='darkgreen', markersize=2,
             markeredgewidth=1, markerfacecolor='none',
             ecolor='springgreen', capsize=2, elinewidth=0.5,
             label='Energy spectra')
plt.ylabel('Counts')
plt.xlabel('Energy [ MeV ]')
plt.xlim(0, 600)
plt.legend(loc="upper right", frameon=False, fontsize=13)

plt.subplot(2,2,2)
plt.plot(energyBinFromFileSpectraFromMCAWithErrors_Value, Fy_Value_Exp,
             marker=".", fillstyle='none',
             linestyle='none',
             markeredgecolor='darkgreen', markersize=2,
             markeredgewidth=1, markerfacecolor='none',
             label='Energy spectra')
plt.ylabel('Counts')
plt.xlabel('Energy [ MeV ]')
plt.xlim(0, 600)
#plt.title('Energy spectra without errors')
plt.legend(loc="upper right", frameon=False, fontsize=13)

prova2 = np.column_stack((energyBinFromFileSpectraFromMCAWithErrors_Value, Fy_Value_Exp))
np.savetxt('filePerGiadaSpettro.out', prova2)

# y versus d(y)
#
plt.subplot(2,2,3)

plt.errorbar(y_Value_Exp, y_dy_Value_Exp,
             y_dy_StandardDeviation_Exp,
             y_StandardDeviation_Exp,
             marker=".", fillstyle='none',
             linestyle='none',
             markeredgecolor='navy', markersize=2,
             markeredgewidth=1, markerfacecolor='none',
             ecolor='royalblue', capsize=2, elinewidth=0.5,
             label=r'y$\cdot$d(y) Exp')
plt.ylabel(r'y$\cdot$d(y)')
plt.xlabel(r'y [ keV/$\mu$m ]')
plt.xlim(0, 50)
#plt.title('yd(y) spectra with errors')
plt.legend(loc="upper right", frameon=False, fontsize=13)

# Write the file for Serena
#
prova = np.column_stack((y_Value_Exp, y_dy_Value_Exp))
np.savetxt('filePerSerena.out', prova)

plt.subplot(2,2,4)
plt.plot(y_Value_Exp, y_Value_Exp*y_Value_Exp*Fy_normalised_Value_Exp/yF_Value_Exp,
             marker=".", fillstyle='none',
             linestyle='none',
             markeredgecolor='navy', markersize=2,
             markeredgewidth=1, markerfacecolor='none',
             label=r'y$\cdot$d(y) Exp')
plt.ylabel(r'y$\cdot$d(y)')
plt.xlabel(r'y [ keV/$\mu$m ]')
plt.xlim(0, 50)
plt.legend(loc="upper right", frameon=False, fontsize=13)
#plt.title('ydy spectra without errors')


plt.draw()
plt.show()