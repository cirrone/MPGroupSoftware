# GAP Cirrone.
# April 22, 2022 - Created to read a phsp file generate in TOPAS MC
# for the ThomsonParabola application
#
# Column 6: particle kinetic energy

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
# Read the file from the Extended Example
#
filePhspThomsonParabola = open('ASCIIOutput.phsp', 'r')
lines_filePhspThomsonParabola = filePhspThomsonParabola.readlines()
filePhspThomsonParabola.close()

kineticEnergy = []


# Loop on all the file lines and slit all the columns
#
for line in lines_filePhspThomsonParabola:
    p = line.split()
# finally vectors are created
    kineticEnergy.append(float(p[5]))


# Created vectors are transformed in Numpy vectors
#
# energyBinFromFileSpectraFromMCA = np.array(energyBinFromFileSpectraFromMCA)
kineticEnergy = np.array(kineticEnergy)



n, bins, patches = plt.hist(kineticEnergy, bins='auto', color='#0504aa',
                            alpha=0.7, rwidth=0.85)



# ***********************************************************************
# PLOTS
#
#figure10 = plt.figure(10, figsize=(7, 7))
#plt.plot(depthExtended , doseFromExtendedNormalised,
 #            linestyle='-',
 #            markeredgecolor='darkgreen', markersize = 4,
 #            markeredgewidth=1, markerfacecolor='none',
 #            label='DDP from Extended example')

#plt.plot(depthFromExp , doseFromExpNormalised,
 #            marker="o", fillstyle='none',
  #           linestyle='none',
   #          markeredgecolor='darkgreen', markersize = 5,
    #         markeredgewidth=1, markerfacecolor='none',
     #        label='DDP from Experiment')

#plt.ylabel('Depth in water [ mm ]')
#plt.xlabel('Dose [ a.u. ]')
#plt.xlim(0, 600)
#plt.ylim(0, 6)
#plt.legend(loc="upper right", frameon=False, fontsize=13)
