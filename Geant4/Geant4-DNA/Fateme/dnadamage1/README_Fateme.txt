=================================================================
                     Geant4 - dnadamage1
=================================================================

---> 1. Introduction


This example  demonstrates 2 ways of the usage of G4ParticleGun shooting primary particles in different cases. These are:

-# 1. Generate simple monoenergetic proton.
-# 2. Show how to sample a tabulated function for energy spectrum.

These usages can be chosen by a UI command as a below:

/gunExample/selectGunAction actionID

 where "actionID" corresponds to above cases. 


----> 2.Primary Generator

There are 2 separate PrimaryGeneratorAction classes(PrimaryGeneratorActionN, N=0,1) which can be used as an independent sample code.
PrimaryGeneratorAction is the class which uses and switches between these 5 concrete action classes. 
Each concrete generator action shoots particles in a distribution described below.

----> 2.1 Generate simple monoenergetic proton.
      
protons are shot as a monoenergetic source.
      

 ----> 2.2 Show how to sample a tabulated function (energy spectrum)
 
Proton source energy is defined as the energy spectrum that is read from the input file(File.txt).   
Energy is sampled from a tabulated function defined in InitFunction().
The function is assumed positive, linear per segment, continuous.
The sampling method is illustrated as RejectAccept() which the data is interpolated linearly.(see Particle Data book, Monte Carlo techniques).



----> 3. How to run the example


Tow macros are available for these two different sources:

-# run_mono.in: shoots 100 protons of 60 MeV

-# run_SPE.in:  shoots 100 protons as a spectrum energy of 60 MeV proton at 24 mm thickness.


----> 4. SIMULATION OUTPUT AND RESULT ANALYSIS                                    

This file can be easily analyzed using the provided ROOT macro 
file analysis.C (and plot.C, molecule.C); The plot.C results in a text file using the SDD format:
 "A new standard DNA damage (SDD) data format" J. Schuemann et al. Published in Rad. Res. 191 (2019) 76-92
 
 4.1. plot.C
 
 -# To count the number of single-string breaks per run, the number of particles must be changed in the  plot.C file.
 
 -# The number of direct and indirect single-string breaks is counted separately in the plot.c file by defining SSBd and SSBi variables Which is finally printed at the end of the SDDformat file.
 
 
 
 * under your ROOT session, type in : .X analysis.C to execute the macro file



----> 5. More information


Specific class is available for source:

* PrimaryGeneratorActionMessenger: defines all UI commands to tune the sources.






