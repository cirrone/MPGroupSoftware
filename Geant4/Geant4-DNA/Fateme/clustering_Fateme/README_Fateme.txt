=================================================================
                     Geant4 - clustering_Fateme
=================================================================

---> 1. Introduction

The clustering example simulates protons tracks in liquid water using Geant4-DNA processes and models.
Energy deposit are clustered with a dedicated clustering algorithm to assess strand breaks. 
This example also demonstrates 2 ways of the usage of G4ParticleGun shooting primary particles in different cases. These are:

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

To get help, run:

> ./clustering -h

In interactive mode, run:

> ./clustering -gui

In batch mode , run:

> ./clustering [-mac run.in] [-mt numberofThreads]


Tow macros are available for these two different sources:

-# run_mono.in: shoots 100 protons of 60 MeV

-# run_SPE.in:  shoots 100 protons as a spectrum energy of 60 MeV proton at 24 mm thickness.




----> 4. More information

-# SteppingAction.cc :  To check the correct definition of the source, the energy of the primary particles is taken and printed as a text file(Spectrum.txt).

-# EventAction.cc: Results of this examle (SSB,DSB, complex SSB, edep) is printed as a text file(Damages.txt)

-# Specific class is available for source:

  * PrimaryGeneratorActionMessenger: defines all UI commands to tune the sources.






