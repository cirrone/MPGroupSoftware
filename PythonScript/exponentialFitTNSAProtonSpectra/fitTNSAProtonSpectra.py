# GAP Cirrone, July 2020
#
# Fit of a typical proton spectra from a TNSA interaction
#
from pylab import *
import chart_studio.plotly as py
import plotly.graph_objs as go
import numpy
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


from decimal import Decimal

# Data from J.S.Green et al. Appl. Phys. Lett. 104, 214101 (2014)
#
energy = np.array([1.2324, 3.2658, 4.5598, 5.5612, 6.5471, 11.1686, 14.9582, 18.0238, 23.9239, 26.25, 29.0537, 30.4247])
numberOfProtonsPerMeV = np.array([1.55e11, 2.57e10, 1.68e10, 7.8e9, 5.2e9, 3.8e9, 1.5e9, 6.088e8, 5.17e8, 2.94e8, 1.92e8, 1.845e8])
numberOfProtonsPerMeV_Log10 = np.log10(numberOfProtonsPerMeV)

# Data from Higginson et al. Nat.Comm. 9, 724 (2018)
#
energyHigginson = np.array([4.28, 8.343, 11.10, 15.548, 20.143, 26.87, 33.38, 42.49, 48.07, 53.51, 59.26, 66, 72.81, 79.39, 86.75, 93.48])
numberOfProtonsPerMeVPerSrHigginson = np.array([1.55e12, 6.90e11, 3.37e11, 1.73e11, 1.15e11, 8.5e10, 6.89e10, 4.90e10, 4.99e10, 2.64e10, 1.46e10, 6.19e9, 3.97e9, 2.22e9, 1.24e9, 9e8])
numberOfProtonsPerMeVPerSrHigginson_Log10 = np.log10(numberOfProtonsPerMeVPerSrHigginson)


# Secondo tipo di fit usando le funzioni LMFIT che dovrebbero essere in grado
# di calcolare il CHI square
#
from lmfit.models import ExpressionModel

# Figure 10
# Data from Green for the 30 MeV maximum proton energy
#
figure10 = plt.figure(10, figsize=(8, 8))

x = energy
a0, a1, a2 = 4.11e11, 0.79, 0
gmod = ExpressionModel("a0*exp(-a1*x)+a2")
result = gmod.fit(numberOfProtonsPerMeV_Log10, x = x, a0 = 4.11e11, a1 = 0.79, a2 = 0)
print(result.fit_report())

plt.plot(energy,numberOfProtonsPerMeV_Log10,
         marker="o", fillstyle='full',
        linestyle='none',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='navy',
        label='Experimental TNSA spectra')

output = np.array([result.chisqr, result.nfree, result.params['a0'].value, result.params['a1'].value, result.params['a2'].value])

plt.plot(x,result.best_fit,
        linestyle='-',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='gray',
        label='Exponential fit: chi2=%5.3f, nfree = %5.3f, a1=%5.3f, a2=%5.3f, a3=%5.3f' % tuple(output))


plt.title('J.S.Green et al. Appl. Phys. Lett. 104, 214101 (2014)')

plt.legend(loc="upper right", frameon=False, fontsize=10)
plt.ylabel(r' Log of number of protons [$\frac{log_{10}N}{MeV}$ ]', size=13)
plt.xlabel('Energy [ MeV ]', size=13)
plt.yscale('log')
plt.xlim(0,35)
plt.ylim(7,13)


figure20 = plt.figure(20, figsize=(8, 8))
x = energy
a0, a1, a2 = 4.11e11, 0.79, 0
gmod = ExpressionModel("a0*exp(-a1*x)+a2")
result = gmod.fit(numberOfProtonsPerMeV, x = x, a0 = 4.11e11, a1 = 0.79, a2 = 0)
print(result.fit_report())

plt.plot(energy,numberOfProtonsPerMeV,
         marker="o", fillstyle='full',
        linestyle='none',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='navy',
        label='Experimental TNSA spectra')


a0 = "{:e}".format(result.params['a0'].value)
a1 = "{:e}".format(result.params['a1'].value)
a2 = "{:e}".format(result.params['a2'].value)

#output = np.array([result.params['a0'].value, result.params['a1'].value, result.params['a2'].value])
outputString = 'a1 = ' + a1 + '   ' + 'a2 = ' + a2


plt.plot(x,result.best_fit,
        linestyle='-',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='gray',
        label=r'Exponential fit: $\frac{dN}{dE} = a0 * e^{-a1*E} + a2$')

plt.text(5, 1.2E11, 'a0 = ' + a0, fontsize=12)
plt.text(5, 1.0E11, 'a1 = ' + a1, fontsize=12)
plt.text(5, 0.8E11, 'a2 = ' + a2, fontsize=12)

plt.title('J.S.Green et al. Appl. Phys. Lett. 104, 214101 (2014)')

plt.legend(loc="upper right", frameon=False, fontsize=10)
plt.ylabel(r' Number of protons [$\frac{N}{MeV}$ ]', size=13)
plt.xlabel('Energy [ MeV ]', size=13)


# Figure 30
# Data from Higginson for the 100 MeV maximum proton energy
#

figure30 = plt.figure(30, figsize=(8, 8))

x = energyHigginson
a0, a1, a2 = 4.11e11, 0.79, 0

gmod = ExpressionModel("a0*exp(-a1*x)+a2")
result = gmod.fit(numberOfProtonsPerMeVPerSrHigginson_Log10, x = x, a0 = 3.6e12, a1 = 0.2020, a2 = 0)
print(result.fit_report())

plt.plot(energyHigginson,numberOfProtonsPerMeVPerSrHigginson_Log10,
         marker="o", fillstyle='full',
        linestyle='none',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='navy',
        label='Experimental TNSA spectra')

output = np.array([result.chisqr, result.nfree, result.params['a0'].value, result.params['a1'].value, result.params['a2'].value])

plt.plot(x,result.best_fit,
        linestyle='-',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='gray',
        label='Exponential fit: chi2=%5.3f, nfree = %5.3f, a1=%5.3f, a2=%5.3f, a3=%5.3f' % tuple(output))

plt.legend(loc="upper right", frameon=False, fontsize=10)
plt.ylabel(r' Log of number of protons [$\frac{log_{10}N}{MeV}$ ]', size=13)
plt.xlabel('Energy [ MeV ]', size=13)
plt.yscale('log')

#plt.xlim(0,35)
#plt.ylim(7,13)



# Fit check delle camere

tensione = np.array([60, 120, 140, 280, 600, 1200, 2400])
carica = np.array([1.73e-8, 3.08e-8, 3.26e-8, 5.5e-8, 7.94e-8, 1.03e-7, 1.22e-7])
from lmfit.models import ExpressionModel

figure100 = plt.figure(100, figsize=(8, 8))

x = tensione
C, S = 1.22e-7, 1393984

gmod = ExpressionModel("(2*C)/(1 + (1 + (2/3)*S*(1/x**2))**(0.5))")
result = gmod.fit(carica, x = x, C = 1.22e7, S = 1393984)
print(result.fit_report())

plt.plot(tensione,carica,
>>>>>>> 085e3e1e3bc016d0931fc4b76522cd5b2f85cadc
         marker="o", fillstyle='full',
        linestyle='none',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='navy',
        label='Experimental TNSA spectra')


output = np.array([result.chisqr, result.nfree, result.params['C'].value, result.params['S'].value])

plt.plot(x,result.best_fit,
        linestyle='-',
        markeredgecolor='navy', markersize=8,
        markeredgewidth=1, markerfacecolor='gray',
        label=' fit: chi2=%5.10f, nfree = %5.3f, C=%5.3f, S=%5.3f' % tuple(output))

#plt.legend(loc="upper left", frameon=False, fontsize=10)
plt.xlabel(r' $\frac{V}{d^2}$', size=13)
plt.ylabel('Carica [ MeV ]', size=13)
