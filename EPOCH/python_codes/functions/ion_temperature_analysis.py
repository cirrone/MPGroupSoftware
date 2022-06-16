import sdf_helper as sh
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import sys
import csv
#from scipy.optimize import curve_fit
import plotly.graph_objs as go
from pylab import *
from plot_spectrum import *
from plot_spectrum_with_fit import *
#import chart_studio.plotly as py
from lmfit.models import ExpressionModel

def ion_temperature_analysis(energy, spectrum, savefile, time, filecsv, particle, time_step):
    x = energy
    x_new = []
    k=0
    for i in x:
        if i > 0.01: 
            x_new.append(i)
        else:
            k=k+1
    y_new = spectrum[k:]
    if len(x_new)==0 : 
        plot_spectrum(x, spectrum, savefile, time, particle)
    else:
        a0, a1 = 1E24, 1E-2
        gmod = ExpressionModel("a0*exp(x*a1)")
        try : 
            result = gmod.fit(y_new, x=x_new, a0=1E24, a1=1E-2)
            a1 = result.params['a1'].value        
            T=-1/a1
            plot_spectrum_with_fit(x, spectrum, savefile, time, result.best_fit, x_new, particle)        
            timestep = time_step
            with open(filecsv, 'a', newline='') as table:
                writer = csv.writer(table)
                writer.writerow([T, timestep])
                table.close()
        except:
            plot_spectrum(x, spectrum, savefile, time, particle)
            
